//
// Generated file, do not edit! Created by opp_msgtool 6.1 from packet.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "packet_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {
namespace fsrv2 {

LinkInfo::LinkInfo()
{
}

LinkInfo::LinkInfo(const LinkInfo& other)
{
    copy(other);
}

LinkInfo::~LinkInfo()
{
}

LinkInfo& LinkInfo::operator=(const LinkInfo& other)
{
    if (this == &other) return *this;
    copy(other);
    return *this;
}

void LinkInfo::copy(const LinkInfo& other)
{
    this->address = other.address;
    this->cost = other.cost;
}

void LinkInfo::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->address);
    doParsimPacking(b,this->cost);
}

void LinkInfo::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->address);
    doParsimUnpacking(b,this->cost);
}

const ::inet::Ipv4Address& LinkInfo::getAddress() const
{
    return this->address;
}

void LinkInfo::setAddress(const ::inet::Ipv4Address& address)
{
    this->address = address;
}

unsigned long LinkInfo::getCost() const
{
    return this->cost;
}

void LinkInfo::setCost(unsigned long cost)
{
    this->cost = cost;
}

class LinkInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_address,
        FIELD_cost,
    };
  public:
    LinkInfoDescriptor();
    virtual ~LinkInfoDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LinkInfoDescriptor)

LinkInfoDescriptor::LinkInfoDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::fsrv2::LinkInfo)), "")
{
    propertyNames = nullptr;
}

LinkInfoDescriptor::~LinkInfoDescriptor()
{
    delete[] propertyNames;
}

bool LinkInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LinkInfo *>(obj)!=nullptr;
}

const char **LinkInfoDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LinkInfoDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LinkInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int LinkInfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_address
        FD_ISEDITABLE,    // FIELD_cost
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *LinkInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "address",
        "cost",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int LinkInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "address") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "cost") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *LinkInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::Ipv4Address",    // FIELD_address
        "unsigned long",    // FIELD_cost
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **LinkInfoDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LinkInfoDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LinkInfoDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void LinkInfoDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LinkInfo'", field);
    }
}

const char *LinkInfoDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LinkInfoDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        case FIELD_address: return pp->getAddress().str();
        case FIELD_cost: return ulong2string(pp->getCost());
        default: return "";
    }
}

void LinkInfoDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        case FIELD_cost: pp->setCost(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkInfo'", field);
    }
}

omnetpp::cValue LinkInfoDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        case FIELD_address: return omnetpp::toAnyPtr(&pp->getAddress()); break;
        case FIELD_cost: return omnetpp::checked_int_cast<omnetpp::intval_t>(pp->getCost());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LinkInfo' as cValue -- field index out of range?", field);
    }
}

void LinkInfoDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        case FIELD_cost: pp->setCost(omnetpp::checked_int_cast<unsigned long>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkInfo'", field);
    }
}

const char *LinkInfoDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr LinkInfoDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        case FIELD_address: return omnetpp::toAnyPtr(&pp->getAddress()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LinkInfoDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkInfo *pp = omnetpp::fromAnyPtr<LinkInfo>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkInfo'", field);
    }
}

// cplusplus {{
LinkInfo::LinkInfo(Ipv4Address address, unsigned long cost) {
    this->address = address;
    this->cost = cost;
}

// Overload the comparison operator for the priority queue
bool LinkInfo::operator>(const LinkInfo& other) const {
    return cost > other.cost;
}
// }}

LinkState::LinkState()
{
}

LinkState::LinkState(const LinkState& other)
{
    copy(other);
}

LinkState::~LinkState()
{
    delete [] this->links;
}

LinkState& LinkState::operator=(const LinkState& other)
{
    if (this == &other) return *this;
    copy(other);
    return *this;
}

void LinkState::copy(const LinkState& other)
{
    this->routerId = other.routerId;
    delete [] this->links;
    this->links = (other.links_arraysize==0) ? nullptr : new ::inet::Ipv4Address[other.links_arraysize];
    links_arraysize = other.links_arraysize;
    for (size_t i = 0; i < links_arraysize; i++) {
        this->links[i] = other.links[i];
    }
    this->timestamp = other.timestamp;
}

void LinkState::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->routerId);
    b->pack(links_arraysize);
    doParsimArrayPacking(b,this->links,links_arraysize);
    doParsimPacking(b,this->timestamp);
}

void LinkState::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->routerId);
    delete [] this->links;
    b->unpack(links_arraysize);
    if (links_arraysize == 0) {
        this->links = nullptr;
    } else {
        this->links = new ::inet::Ipv4Address[links_arraysize];
        doParsimArrayUnpacking(b,this->links,links_arraysize);
    }
    doParsimUnpacking(b,this->timestamp);
}

const ::inet::Ipv4Address& LinkState::getRouterId() const
{
    return this->routerId;
}

void LinkState::setRouterId(const ::inet::Ipv4Address& routerId)
{
    this->routerId = routerId;
}

size_t LinkState::getLinksArraySize() const
{
    return links_arraysize;
}

const ::inet::Ipv4Address& LinkState::getLinks(size_t k) const
{
    if (k >= links_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)links_arraysize, (unsigned long)k);
    return this->links[k];
}

void LinkState::setLinksArraySize(size_t newSize)
{
    ::inet::Ipv4Address *links2 = (newSize==0) ? nullptr : new ::inet::Ipv4Address[newSize];
    size_t minSize = links_arraysize < newSize ? links_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        links2[i] = this->links[i];
    delete [] this->links;
    this->links = links2;
    links_arraysize = newSize;
}

void LinkState::setLinks(size_t k, const ::inet::Ipv4Address& links)
{
    if (k >= links_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)links_arraysize, (unsigned long)k);
    this->links[k] = links;
}

void LinkState::insertLinks(size_t k, const ::inet::Ipv4Address& links)
{
    if (k > links_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)links_arraysize, (unsigned long)k);
    size_t newSize = links_arraysize + 1;
    ::inet::Ipv4Address *links2 = new ::inet::Ipv4Address[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        links2[i] = this->links[i];
    links2[k] = links;
    for (i = k + 1; i < newSize; i++)
        links2[i] = this->links[i-1];
    delete [] this->links;
    this->links = links2;
    links_arraysize = newSize;
}

void LinkState::appendLinks(const ::inet::Ipv4Address& links)
{
    insertLinks(links_arraysize, links);
}

void LinkState::eraseLinks(size_t k)
{
    if (k >= links_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)links_arraysize, (unsigned long)k);
    size_t newSize = links_arraysize - 1;
    ::inet::Ipv4Address *links2 = (newSize == 0) ? nullptr : new ::inet::Ipv4Address[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        links2[i] = this->links[i];
    for (i = k; i < newSize; i++)
        links2[i] = this->links[i+1];
    delete [] this->links;
    this->links = links2;
    links_arraysize = newSize;
}

::omnetpp::simtime_t LinkState::getTimestamp() const
{
    return this->timestamp;
}

void LinkState::setTimestamp(::omnetpp::simtime_t timestamp)
{
    this->timestamp = timestamp;
}

class LinkStateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_routerId,
        FIELD_links,
        FIELD_timestamp,
    };
  public:
    LinkStateDescriptor();
    virtual ~LinkStateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LinkStateDescriptor)

LinkStateDescriptor::LinkStateDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::fsrv2::LinkState)), "")
{
    propertyNames = nullptr;
}

LinkStateDescriptor::~LinkStateDescriptor()
{
    delete[] propertyNames;
}

bool LinkStateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LinkState *>(obj)!=nullptr;
}

const char **LinkStateDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = { "packetData",  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LinkStateDescriptor::getProperty(const char *propertyName) const
{
    if (!strcmp(propertyName, "packetData")) return "";
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LinkStateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int LinkStateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_routerId
        FD_ISARRAY | FD_ISRESIZABLE,    // FIELD_links
        FD_ISEDITABLE,    // FIELD_timestamp
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *LinkStateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "routerId",
        "links",
        "timestamp",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int LinkStateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "routerId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "links") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *LinkStateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::Ipv4Address",    // FIELD_routerId
        "inet::Ipv4Address",    // FIELD_links
        "omnetpp::simtime_t",    // FIELD_timestamp
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **LinkStateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LinkStateDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LinkStateDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_links: return pp->getLinksArraySize();
        default: return 0;
    }
}

void LinkStateDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_links: pp->setLinksArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LinkState'", field);
    }
}

const char *LinkStateDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LinkStateDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_routerId: return pp->getRouterId().str();
        case FIELD_links: return pp->getLinks(i).str();
        case FIELD_timestamp: return simtime2string(pp->getTimestamp());
        default: return "";
    }
}

void LinkStateDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_timestamp: pp->setTimestamp(string2simtime(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkState'", field);
    }
}

omnetpp::cValue LinkStateDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_routerId: return omnetpp::toAnyPtr(&pp->getRouterId()); break;
        case FIELD_links: return omnetpp::toAnyPtr(&pp->getLinks(i)); break;
        case FIELD_timestamp: return pp->getTimestamp().dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LinkState' as cValue -- field index out of range?", field);
    }
}

void LinkStateDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkState'", field);
    }
}

const char *LinkStateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr LinkStateDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        case FIELD_routerId: return omnetpp::toAnyPtr(&pp->getRouterId()); break;
        case FIELD_links: return omnetpp::toAnyPtr(&pp->getLinks(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LinkStateDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LinkState *pp = omnetpp::fromAnyPtr<LinkState>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LinkState'", field);
    }
}

Register_Class(LSUPacket)

LSUPacket::LSUPacket() : ::inet::FieldsChunk()
{
}

LSUPacket::LSUPacket(const LSUPacket& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

LSUPacket::~LSUPacket()
{
    delete [] this->linkStates;
}

LSUPacket& LSUPacket::operator=(const LSUPacket& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void LSUPacket::copy(const LSUPacket& other)
{
    this->srcAddress = other.srcAddress;
    delete [] this->linkStates;
    this->linkStates = (other.linkStates_arraysize==0) ? nullptr : new LinkState[other.linkStates_arraysize];
    linkStates_arraysize = other.linkStates_arraysize;
    for (size_t i = 0; i < linkStates_arraysize; i++) {
        this->linkStates[i] = other.linkStates[i];
    }
}

void LSUPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->srcAddress);
    b->pack(linkStates_arraysize);
    doParsimArrayPacking(b,this->linkStates,linkStates_arraysize);
}

void LSUPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->srcAddress);
    delete [] this->linkStates;
    b->unpack(linkStates_arraysize);
    if (linkStates_arraysize == 0) {
        this->linkStates = nullptr;
    } else {
        this->linkStates = new LinkState[linkStates_arraysize];
        doParsimArrayUnpacking(b,this->linkStates,linkStates_arraysize);
    }
}

const ::inet::Ipv4Address& LSUPacket::getSrcAddress() const
{
    return this->srcAddress;
}

void LSUPacket::setSrcAddress(const ::inet::Ipv4Address& srcAddress)
{
    handleChange();
    this->srcAddress = srcAddress;
}

size_t LSUPacket::getLinkStatesArraySize() const
{
    return linkStates_arraysize;
}

const LinkState& LSUPacket::getLinkStates(size_t k) const
{
    if (k >= linkStates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)linkStates_arraysize, (unsigned long)k);
    return this->linkStates[k];
}

void LSUPacket::setLinkStatesArraySize(size_t newSize)
{
    handleChange();
    LinkState *linkStates2 = (newSize==0) ? nullptr : new LinkState[newSize];
    size_t minSize = linkStates_arraysize < newSize ? linkStates_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        linkStates2[i] = this->linkStates[i];
    delete [] this->linkStates;
    this->linkStates = linkStates2;
    linkStates_arraysize = newSize;
}

void LSUPacket::setLinkStates(size_t k, const LinkState& linkStates)
{
    if (k >= linkStates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)linkStates_arraysize, (unsigned long)k);
    handleChange();
    this->linkStates[k] = linkStates;
}

void LSUPacket::insertLinkStates(size_t k, const LinkState& linkStates)
{
    if (k > linkStates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)linkStates_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = linkStates_arraysize + 1;
    LinkState *linkStates2 = new LinkState[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        linkStates2[i] = this->linkStates[i];
    linkStates2[k] = linkStates;
    for (i = k + 1; i < newSize; i++)
        linkStates2[i] = this->linkStates[i-1];
    delete [] this->linkStates;
    this->linkStates = linkStates2;
    linkStates_arraysize = newSize;
}

void LSUPacket::appendLinkStates(const LinkState& linkStates)
{
    insertLinkStates(linkStates_arraysize, linkStates);
}

void LSUPacket::eraseLinkStates(size_t k)
{
    if (k >= linkStates_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)linkStates_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = linkStates_arraysize - 1;
    LinkState *linkStates2 = (newSize == 0) ? nullptr : new LinkState[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        linkStates2[i] = this->linkStates[i];
    for (i = k; i < newSize; i++)
        linkStates2[i] = this->linkStates[i+1];
    delete [] this->linkStates;
    this->linkStates = linkStates2;
    linkStates_arraysize = newSize;
}

class LSUPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcAddress,
        FIELD_linkStates,
    };
  public:
    LSUPacketDescriptor();
    virtual ~LSUPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(LSUPacketDescriptor)

LSUPacketDescriptor::LSUPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::fsrv2::LSUPacket)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

LSUPacketDescriptor::~LSUPacketDescriptor()
{
    delete[] propertyNames;
}

bool LSUPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LSUPacket *>(obj)!=nullptr;
}

const char **LSUPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *LSUPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int LSUPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int LSUPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_srcAddress
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_linkStates
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *LSUPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcAddress",
        "linkStates",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int LSUPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcAddress") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "linkStates") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *LSUPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::Ipv4Address",    // FIELD_srcAddress
        "inet::fsrv2::LinkState",    // FIELD_linkStates
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **LSUPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *LSUPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int LSUPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        case FIELD_linkStates: return pp->getLinkStatesArraySize();
        default: return 0;
    }
}

void LSUPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        case FIELD_linkStates: pp->setLinkStatesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'LSUPacket'", field);
    }
}

const char *LSUPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LSUPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        case FIELD_srcAddress: return pp->getSrcAddress().str();
        case FIELD_linkStates: return "";
        default: return "";
    }
}

void LSUPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LSUPacket'", field);
    }
}

omnetpp::cValue LSUPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        case FIELD_srcAddress: return omnetpp::toAnyPtr(&pp->getSrcAddress()); break;
        case FIELD_linkStates: return omnetpp::toAnyPtr(&pp->getLinkStates(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'LSUPacket' as cValue -- field index out of range?", field);
    }
}

void LSUPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LSUPacket'", field);
    }
}

const char *LSUPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_linkStates: return omnetpp::opp_typename(typeid(LinkState));
        default: return nullptr;
    };
}

omnetpp::any_ptr LSUPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        case FIELD_srcAddress: return omnetpp::toAnyPtr(&pp->getSrcAddress()); break;
        case FIELD_linkStates: return omnetpp::toAnyPtr(&pp->getLinkStates(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void LSUPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    LSUPacket *pp = omnetpp::fromAnyPtr<LSUPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'LSUPacket'", field);
    }
}

ScopePeriod::ScopePeriod()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const ScopePeriod& a)
{
    doParsimPacking(b,a.scope);
    doParsimPacking(b,a.period);
}

void __doUnpacking(omnetpp::cCommBuffer *b, ScopePeriod& a)
{
    doParsimUnpacking(b,a.scope);
    doParsimUnpacking(b,a.period);
}

class ScopePeriodDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_scope,
        FIELD_period,
    };
  public:
    ScopePeriodDescriptor();
    virtual ~ScopePeriodDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ScopePeriodDescriptor)

ScopePeriodDescriptor::ScopePeriodDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::fsrv2::ScopePeriod)), "")
{
    propertyNames = nullptr;
}

ScopePeriodDescriptor::~ScopePeriodDescriptor()
{
    delete[] propertyNames;
}

bool ScopePeriodDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ScopePeriod *>(obj)!=nullptr;
}

const char **ScopePeriodDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ScopePeriodDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ScopePeriodDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int ScopePeriodDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_scope
        FD_ISEDITABLE,    // FIELD_period
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *ScopePeriodDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "scope",
        "period",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int ScopePeriodDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "scope") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "period") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *ScopePeriodDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",    // FIELD_scope
        "omnetpp::simtime_t",    // FIELD_period
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **ScopePeriodDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ScopePeriodDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ScopePeriodDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ScopePeriodDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ScopePeriod'", field);
    }
}

const char *ScopePeriodDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ScopePeriodDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        case FIELD_scope: return ulong2string(pp->scope);
        case FIELD_period: return simtime2string(pp->period);
        default: return "";
    }
}

void ScopePeriodDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        case FIELD_scope: pp->scope = string2ulong(value); break;
        case FIELD_period: pp->period = string2simtime(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopePeriod'", field);
    }
}

omnetpp::cValue ScopePeriodDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        case FIELD_scope: return (omnetpp::intval_t)(pp->scope);
        case FIELD_period: return pp->period.dbl();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ScopePeriod' as cValue -- field index out of range?", field);
    }
}

void ScopePeriodDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        case FIELD_scope: pp->scope = omnetpp::checked_int_cast<unsigned int>(value.intValue()); break;
        case FIELD_period: pp->period = value.doubleValue(); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopePeriod'", field);
    }
}

const char *ScopePeriodDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ScopePeriodDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ScopePeriodDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopePeriod *pp = omnetpp::fromAnyPtr<ScopePeriod>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopePeriod'", field);
    }
}

Register_Class(ScopesParam)

ScopesParam::ScopesParam(const char *name) : ::omnetpp::cOwnedObject(name)
{
}

ScopesParam::ScopesParam(const ScopesParam& other) : ::omnetpp::cOwnedObject(other)
{
    copy(other);
}

ScopesParam::~ScopesParam()
{
    delete [] this->scopes;
}

ScopesParam& ScopesParam::operator=(const ScopesParam& other)
{
    if (this == &other) return *this;
    ::omnetpp::cOwnedObject::operator=(other);
    copy(other);
    return *this;
}

void ScopesParam::copy(const ScopesParam& other)
{
    delete [] this->scopes;
    this->scopes = (other.scopes_arraysize==0) ? nullptr : new ScopePeriod[other.scopes_arraysize];
    scopes_arraysize = other.scopes_arraysize;
    for (size_t i = 0; i < scopes_arraysize; i++) {
        this->scopes[i] = other.scopes[i];
    }
}

void ScopesParam::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cOwnedObject::parsimPack(b);
    b->pack(scopes_arraysize);
    doParsimArrayPacking(b,this->scopes,scopes_arraysize);
}

void ScopesParam::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cOwnedObject::parsimUnpack(b);
    delete [] this->scopes;
    b->unpack(scopes_arraysize);
    if (scopes_arraysize == 0) {
        this->scopes = nullptr;
    } else {
        this->scopes = new ScopePeriod[scopes_arraysize];
        doParsimArrayUnpacking(b,this->scopes,scopes_arraysize);
    }
}

size_t ScopesParam::getScopesArraySize() const
{
    return scopes_arraysize;
}

const ScopePeriod& ScopesParam::getScopes(size_t k) const
{
    if (k >= scopes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scopes_arraysize, (unsigned long)k);
    return this->scopes[k];
}

void ScopesParam::setScopesArraySize(size_t newSize)
{
    ScopePeriod *scopes2 = (newSize==0) ? nullptr : new ScopePeriod[newSize];
    size_t minSize = scopes_arraysize < newSize ? scopes_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        scopes2[i] = this->scopes[i];
    delete [] this->scopes;
    this->scopes = scopes2;
    scopes_arraysize = newSize;
}

void ScopesParam::setScopes(size_t k, const ScopePeriod& scopes)
{
    if (k >= scopes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scopes_arraysize, (unsigned long)k);
    this->scopes[k] = scopes;
}

void ScopesParam::insertScopes(size_t k, const ScopePeriod& scopes)
{
    if (k > scopes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scopes_arraysize, (unsigned long)k);
    size_t newSize = scopes_arraysize + 1;
    ScopePeriod *scopes2 = new ScopePeriod[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        scopes2[i] = this->scopes[i];
    scopes2[k] = scopes;
    for (i = k + 1; i < newSize; i++)
        scopes2[i] = this->scopes[i-1];
    delete [] this->scopes;
    this->scopes = scopes2;
    scopes_arraysize = newSize;
}

void ScopesParam::appendScopes(const ScopePeriod& scopes)
{
    insertScopes(scopes_arraysize, scopes);
}

void ScopesParam::eraseScopes(size_t k)
{
    if (k >= scopes_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scopes_arraysize, (unsigned long)k);
    size_t newSize = scopes_arraysize - 1;
    ScopePeriod *scopes2 = (newSize == 0) ? nullptr : new ScopePeriod[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        scopes2[i] = this->scopes[i];
    for (i = k; i < newSize; i++)
        scopes2[i] = this->scopes[i+1];
    delete [] this->scopes;
    this->scopes = scopes2;
    scopes_arraysize = newSize;
}

class ScopesParamDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_scopes,
    };
  public:
    ScopesParamDescriptor();
    virtual ~ScopesParamDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ScopesParamDescriptor)

ScopesParamDescriptor::ScopesParamDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::fsrv2::ScopesParam)), "omnetpp::cOwnedObject")
{
    propertyNames = nullptr;
}

ScopesParamDescriptor::~ScopesParamDescriptor()
{
    delete[] propertyNames;
}

bool ScopesParamDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ScopesParam *>(obj)!=nullptr;
}

const char **ScopesParamDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ScopesParamDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ScopesParamDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int ScopesParamDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_scopes
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *ScopesParamDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "scopes",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int ScopesParamDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "scopes") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *ScopesParamDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::fsrv2::ScopePeriod",    // FIELD_scopes
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **ScopesParamDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ScopesParamDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ScopesParamDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        case FIELD_scopes: return pp->getScopesArraySize();
        default: return 0;
    }
}

void ScopesParamDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        case FIELD_scopes: pp->setScopesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ScopesParam'", field);
    }
}

const char *ScopesParamDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ScopesParamDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        case FIELD_scopes: return "";
        default: return "";
    }
}

void ScopesParamDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopesParam'", field);
    }
}

omnetpp::cValue ScopesParamDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        case FIELD_scopes: return omnetpp::toAnyPtr(&pp->getScopes(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ScopesParam' as cValue -- field index out of range?", field);
    }
}

void ScopesParamDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopesParam'", field);
    }
}

const char *ScopesParamDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_scopes: return omnetpp::opp_typename(typeid(ScopePeriod));
        default: return nullptr;
    };
}

omnetpp::any_ptr ScopesParamDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        case FIELD_scopes: return omnetpp::toAnyPtr(&pp->getScopes(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ScopesParamDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ScopesParam *pp = omnetpp::fromAnyPtr<ScopesParam>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ScopesParam'", field);
    }
}

}  // namespace fsrv2
}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

