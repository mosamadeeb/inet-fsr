import os
from collections import defaultdict
import pandas as pd
import numpy as np
from scipy import stats
import sys
import matplotlib.pyplot as plt

# The CSV file should be exported from the vectors tab of the .anf analysis file in omnetpp
# This is the filter expression for selecting the vectors
# (module =~ "*.hostA.app[0]" AND name =~ "packetSent:vector(packetBytes)") OR (module =~ "*.hostB.app[0]" AND (name =~ "packetReceived:vector(packetBytes)" OR name =~ "endToEndDelay:vector" OR name =~ "throughput:vector")) OR (module =~"*.mac" AND name =~ "sentDownPk:vector(packetBytes)")

extracted_vectors = defaultdict(lambda: defaultdict(list))

throughput_str = 'End-to-end throughput'
delay_str = 'End-to-end delay'
delivery_ratio_str = 'Packet delivery ratio'
data_bit_transmitted_str = 'Data bits transmitted per data bit delivered'
control_bit_transmitted_str = 'Control bits transmitted per data bit delivered'

def calculate_confidence_interval(data, confidence=0.95):
    if len(data) == 0:
        return np.nan, np.nan
    mean = np.mean(data)
    se = stats.sem(data)  # Standard error of the mean
    ci = stats.t.ppf((1 + confidence) / 2., len(data) - 1) * se  # Confidence interval
    return mean, ci


g_itervar_name = None
g_itervar_replication_map = None


def extract_vectors(file_name, test_name):
    # Read the CSV file
    df = pd.read_csv(file_name)

    # Iteration variable
    # Example row:
    # FsrTest1-1-20250611-01:18:35-1356476,itervar,,,bitrate,4,,
    # Run number is between the first two dashes in the 'run' column
    # Iteration variable name is in the 'attrname' column, and its value in the 'attrvalue' column
    itervar_rows = df[df['type'] == 'itervar']

    # Extract the run numbers, iteration variable name and values
    itervar_name = itervar_rows['attrname'].iloc[0]
    itervar_values = [(row['run'].split('-')[1], row['attrvalue']) for index, row in itervar_rows.iterrows()]

    # If an itervar value appears multiple times, then those runs are replications.
    # Map the itervar values to a dictionary where the key is the itervar value and the value is a list of run numbers.
    itervar_replication_map = defaultdict(list)
    for run, value in itervar_values:
        val = int(value)

        if itervar_name == 'nodeCount' and val == 6:
            # Skip this as the results were not satisfactory
            continue

        itervar_replication_map[val].append(run)

    for k in itervar_replication_map:
        itervar_replication_map[k].sort()

    # Set the global variables for the iteration variable name and its replication map
    global g_itervar_name, g_itervar_replication_map
    if g_itervar_name is None or g_itervar_replication_map is None:
        g_itervar_name = itervar_name
        g_itervar_replication_map = itervar_replication_map

    # Select all rows with 'vector' in the 'type' column
    df_vector = df[df['type'].str.contains('vector')]

    # Based on the run number of each vector, extract the 'module' and 'name' columns into a tuple, and the 'vectime', 'vecvalue' columns into another tuple.
    # Map the second tuple to the first tuple in a dictionary. There should be one dictionary entry for each run number.
    vector_data = defaultdict(dict)
    for index, row in df_vector.iterrows():
        run_number = row['run'].split('-')[1]
        vector_data[run_number][(row['module'], row['name'])] = (row['vectime'], row['vecvalue'])

    for itervar_value in sorted(itervar_replication_map.keys()):
        runs = itervar_replication_map[itervar_value]

        print(f"Iteration variable: {itervar_name} = {itervar_value}")
        print(f"Runs: {', '.join(runs)}")
        
        extracted_values = {}
        extracted_values['throughput'] = []
        extracted_values['endToEndDelay'] = []
        extracted_values['packetSent'] = []
        extracted_values['packetReceived'] = []
        total_data_bits_transmitted = []
        total_control_bits_transmitted = []

        for run in runs:
            if run not in vector_data:
                continue

            # Find the message length for this run
            # Example row:
            # FsrTest1-1-20250611-01:18:35-1356476,config,,,*.hostA.app[0].messageLength,100B,,
            message_length_row = df[(df['attrname'].str.endswith('.messageLength')) & (df['run'].str.split('-').str[1] == run)]
            msg_len = int(message_length_row['attrvalue'].iloc[0][:-1]) if not message_length_row.empty else 100
            data_packet_length = msg_len + 51  # 51 bytes is the overhead for IP packets (in our simulation)

            runDataBytesTransmitted = 0
            runControlBytesTransmitted = 0

            # print(f"  Run {run}: messageLength = {msg_len}")

            for (module, name), (vectime, vecvalue) in vector_data[run].items():
                name = name.split(':')[0]

                # Edge case with empty list that gets converted to nan
                if 'split' not in dir(vecvalue):
                    vecvalue = np.array([])
                else:
                    vecvalue = np.array([float(x) for x in vecvalue.split()])

                if name in ['throughput', 'endToEndDelay']:
                    if module != 'FsrTestNet.hostB.app[0]':
                        continue

                    # Calculate the mean by summing the values and dividing by the number of entries
                    avg_value = np.mean(vecvalue)
                    extracted_values[name].append(avg_value)
                    # print(f"    {module}.{name}: {avg_value}")

                elif name in ['packetSent', 'packetReceived']:
                    if name == 'packetSent' and module != 'FsrTestNet.hostA.app[0]':
                        continue

                    if name == 'packetReceived' and module != 'FsrTestNet.hostB.app[0]':
                        continue

                    # Just count the number of entries
                    count_value = len(vecvalue)
                    extracted_values[name].append(count_value)
                    # print(f"    {module}.{name}: {count_value}")
                elif name == 'sentDownPk':
                    # Track all data packets transmitted. Data packets are those with size equal to the message length (+ IP header).
                    data_packets = vecvalue[vecvalue == data_packet_length]
                    runDataBytesTransmitted += len(data_packets) * msg_len

                    # Other packets are control packets.
                    control_packets = vecvalue[vecvalue != data_packet_length]
                    runControlBytesTransmitted += control_packets.sum()

            total_data_bits_transmitted.append(runDataBytesTransmitted * 8)  # Convert bytes to bits
            total_control_bits_transmitted.append(runControlBytesTransmitted * 8)

        # Convert the extracted values to numpy arrays for statistical analysis
        throughput = np.array(extracted_values['throughput'])
        end_to_end_delay = np.array(extracted_values['endToEndDelay'])
        packet_sent = np.array(extracted_values['packetSent'])
        packet_received = np.array(extracted_values['packetReceived'])
        total_data_bits_transmitted = np.array(total_data_bits_transmitted)
        total_control_bits_transmitted = np.array(total_control_bits_transmitted)

        # Print the results
        if len(throughput) != 0:
            print(f"  Throughput: {throughput.mean()/1000:.2f}kbps ± {throughput.std()/1000:.2f}kbps")
            extracted_vectors[throughput_str][test_name].append(calculate_confidence_interval(throughput))

        if len(end_to_end_delay) != 0:
            print(f"  End-to-end delay: {end_to_end_delay.mean()*1000:.2f}ms ± {end_to_end_delay.std()*1000:.2f}ms")
            extracted_vectors[delay_str][test_name].append(calculate_confidence_interval(end_to_end_delay))

        if len(packet_sent) != 0:
            print(f"  Packets sent: {packet_sent.sum()} (mean: {packet_sent.mean():.2f} ± {packet_sent.std():.2f})")

        if len(packet_received) != 0:
            print(f"  Packets received: {packet_received.sum()} (mean: {packet_received.mean():.2f} ± {packet_received.std():.2f})")

        if len(packet_sent) != 0 and len(packet_received) != 0:
            # Calculate delivery ratio per run.
            delivery_ratio = packet_received / packet_sent
            print(f"  Delivery ratio: {delivery_ratio.mean():.2f} ± {delivery_ratio.std():.2f}")
            extracted_vectors[delivery_ratio_str][test_name].append(calculate_confidence_interval(delivery_ratio))

        # Calculate total data bits delivered
        total_data_bits_delivered = packet_received * msg_len * 8  # Convert bytes to bits

        if len(total_data_bits_transmitted) != 0 and len(packet_received) != 0:
            # Calculate data bit transmitted to data bit delivered ratio
            data_bit_transmitted_ratio = total_data_bits_transmitted / total_data_bits_delivered
            print(f"  Data bits transmitted to data bits delivered ratio: {data_bit_transmitted_ratio.mean():.2f} ± {data_bit_transmitted_ratio.std():.2f}")
            extracted_vectors[data_bit_transmitted_str][test_name].append(calculate_confidence_interval(data_bit_transmitted_ratio))

        if len(total_control_bits_transmitted) != 0 and len(packet_received) != 0:
            # Calculate control bit transmitted to data bit delivered ratio
            control_bit_transmitted_ratio = total_control_bits_transmitted / total_data_bits_delivered
            print(f"  Control bits transmitted to data bits delivered ratio: {control_bit_transmitted_ratio.mean():.2f} ± {control_bit_transmitted_ratio.std():.2f}")
            extracted_vectors[control_bit_transmitted_str][test_name].append(calculate_confidence_interval(control_bit_transmitted_ratio))


def plot_figures(extracted_vectors, itervar_name, itervar_replication_map, output_dir):
    itervar_name_map = {
        'bitrate': 'Bitrate (Mbps)',
        'messageLength': 'Data size (bytes)',
        'nodeCount': 'Node count',
        'sendInterval': 'Packet interval mean (ms)',
        'speed': 'Mobility speed (mps)',
    }

    for var_name, test_vectors in extracted_vectors.items():
        plt.figure(figsize=(10, 6))

        x_positions = np.array(sorted(itervar_replication_map.keys()))

        for test_name, values in test_vectors.items():
            means = [v[0] for v in values]
            cis = [v[1] for v in values]

            if var_name == throughput_str:
                means = [x / 1000 for x in means]
                cis = [x / 1000 for x in cis]
            elif var_name == delay_str:
                means = [x * 1000 for x in means]
                cis = [x * 1000 for x in cis]

            # Plotting the mean values as a line
            plt.plot(x_positions, means, marker='o', linestyle='-', label=test_name)

            # Filling the confidence interval around the mean
            upper_bound = [means[i] + cis[i] for i in range(len(means))]
            lower_bound = [means[i] - cis[i] for i in range(len(means))]
            plt.fill_between(x_positions, lower_bound, upper_bound, alpha=0.2)

        plt.xticks(x_positions, sorted(itervar_replication_map.keys()), rotation=45)
        plt.title(var_name)

        plt.xlabel(itervar_name_map.get(itervar_name, itervar_name))

        if var_name == throughput_str:
            plt.ylabel('Throughput (Kbps)')
        elif var_name == delay_str:
            plt.ylabel('Delay (ms)')
        else:
            plt.ylabel('Ratio')

        plt.tight_layout()
        plt.grid(axis='y', alpha=0.3)
        plt.legend()

        # Improve y-axis ticks
        ax = plt.gca()
        ax.yaxis.set_major_locator(plt.MaxNLocator(8))  # Adjust the maximum number of ticks

        # Save the figure
        plt.savefig(f'{output_dir}/{var_name.replace("/", " per ").replace(" ", "_").lower()}.png', dpi=300)
        plt.show()


if __name__ == '__main__':
    if len(sys.argv) > 1:
        file_name = sys.argv[1]
    else:
        print(f"Usage: {sys.argv[0]} <path_to_csv_file>")
        sys.exit(1)

    if not os.path.exists(file_name):
        print(f"File not found: {file_name}")
        sys.exit(1)
    
    test_name = os.path.basename(file_name).split(".")[0]
    output_dir = f'figures/{test_name}'

    # Create the directory for storing figures
    os.makedirs(output_dir, exist_ok=True)

    file_prefix = file_name[:-4]
    for file_suffix, test_name in [('-fsr2.csv', 'FSR(0.25s, 0.50s, 0.75s)'), ('.csv', 'FSR(0.50s, 0.75s, 1.00s)'), ('-fsr3.csv', 'FSR(0.75s, 1.50s, 2.25s)'), ('-gsr.csv', 'GSR(0.5s)')]:
    # for file_suffix, test_name in [('-fsr3.csv', 'FSR(0.75s, 1.50s, 2.25s)')]:
        file = f'{file_prefix}{file_suffix}'
        if os.path.exists(file):
            extract_vectors(file, test_name)

    plot_figures(extracted_vectors, g_itervar_name, g_itervar_replication_map, output_dir)
