from collections import defaultdict
import pandas as pd
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt

# Take first argument as the file name
import sys
if len(sys.argv) > 1:
    file_name = sys.argv[1]
else:
    file_name = 'results/tput.csv'

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
    itervar_replication_map[value].append(run)

for k in itervar_replication_map:
    itervar_replication_map[k].sort()

# Select all rows with 'vector' in the 'type' column
df_vector = df[df['type'].str.contains('vector')]

# Based on the run number of each vector, extract the 'module' and 'name' columns into a tuple, and the 'vectime', 'vecvalue' columns into another tuple.
# Map the second tuple to the first tuple in a dictionary. There should be one dictionary entry for each run number.
vector_data = defaultdict(dict)
for index, row in df_vector.iterrows():
    run_number = row['run'].split('-')[1]
    vector_data[run_number][(row['module'], row['name'])] = (row['vectime'], row['vecvalue'])

for itervar_value, runs in itervar_replication_map.items():
    print(f"Iteration variable: {itervar_name} = {itervar_value}")
    print(f"Runs: {', '.join(runs)}")
    
    extracted_values = {}
    extracted_values['throughput'] = []
    extracted_values['endToEndDelay'] = []
    extracted_values['packetSent'] = []
    extracted_values['packetReceived'] = []
    extracted_values['totalDataBitsTransmitted'] = []
    extracted_values['totalControlBitsTransmitted'] = []

    for run in runs:
        # Find the message length for this run
        # Example row:
        # FsrTest1-1-20250611-01:18:35-1356476,config,,,*.hostA.app[0].messageLength,100B,,
        message_length_row = df[(df['attrname'].str.endswith('.messageLength')) & (df['run'].str.split('-').str[1] == run)]
        msg_len = int(message_length_row['attrvalue'].iloc[0][:-1]) if not message_length_row.empty else 100

        if run in vector_data:
            print(f"  Run {run}: messageLength = {msg_len}")

            for (module, name), (vectime, vecvalue) in vector_data[run].items():
                name = name.split(':')[0]

                # vecvalue is a string with space separated values
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
                    count_value = np.sum(vecvalue)
                    extracted_values[name].append(count_value)
                    # print(f"    {module}.{name}: {count_value}")
                elif name == 'sentDownPk':
                    # Track all data packets transmitted. Data packets are those with size equal to the message length.
                    data_packets = vecvalue[vecvalue == msg_len]
                    extracted_values['totalDataBitsTransmitted'].append(len(data_packets) * msg_len * 8)  # Convert bytes to bits

                    # Other packets are control packets.
                    control_packets = vecvalue[vecvalue != msg_len]
                    extracted_values['totalControlBitsTransmitted'].append(control_packets.sum() * 8)  # Convert bytes to bits

    # Convert the extracted values to numpy arrays for statistical analysis
    throughput = np.array(extracted_values['throughput'])
    end_to_end_delay = np.array(extracted_values['endToEndDelay'])
    packet_sent = np.array(extracted_values['packetSent'])
    packet_received = np.array(extracted_values['packetReceived'])
    total_data_bits_transmitted = np.array(extracted_values['totalDataBitsTransmitted'])
    total_control_bits_transmitted = np.array(extracted_values['totalControlBitsTransmitted'])

    # Print the results
    if len(throughput) != 0:
        print(f"  Throughput: {throughput.mean():.2f} ± {throughput.std():.2f}")

    if len(end_to_end_delay) != 0:
        print(f"  End-to-end delay: {end_to_end_delay.mean():.2f} ± {end_to_end_delay.std():.2f}")
    
    if len(packet_sent) != 0:
        print(f"  Packets sent: {packet_sent.sum()} (mean: {packet_sent.mean():.2f} ± {packet_sent.std():.2f})")

    if len(packet_received) != 0:
        print(f"  Packets received: {packet_received.sum()} (mean: {packet_received.mean():.2f} ± {packet_received.std():.2f})")

    if len(packet_sent) != 0 and len(packet_received) != 0:
        # Calculate delivery ratio per run.
        delivery_ratio = packet_received / packet_sent
        print(f"  Delivery ratio: {delivery_ratio.mean():.2f} ± {delivery_ratio.std():.2f}")

    # Calculate total data bits delivered
    total_data_bits_delivered = packet_received * msg_len * 8  # Convert bytes to bits

    if len(total_data_bits_transmitted) != 0 and len(packet_received) != 0:
        # Calculate data bit transmitted to data bit delivered ratio
        data_bit_transmitted_ratio = total_data_bits_transmitted / total_data_bits_delivered
        print(f"  Data bits transmitted to data bits delivered ratio: {data_bit_transmitted_ratio.mean():.2f} ± {data_bit_transmitted_ratio.std():.2f}")

    if len(total_control_bits_transmitted) != 0 and len(packet_received) != 0:
        # Calculate control bit transmitted to data bit delivered ratio
        control_bit_transmitted_ratio = total_control_bits_transmitted / total_data_bits_delivered
        print(f"  Control bits transmitted to data bits delivered ratio: {control_bit_transmitted_ratio.mean():.2f} ± {control_bit_transmitted_ratio.std():.2f}")
