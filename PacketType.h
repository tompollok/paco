// Package Container library (paco)

// Copyright 2017 (c) Thomas Pollok (tom dot pollok at gmail dot com)

// https://github.com/tompollok/paco

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PACKET_TYPE_H
#define PACKET_TYPE_H


#include <stdio.h>
#include <iostream>
#include <typeinfo>

#if __GNUC__
    #include <cxxabi.h>
#endif



#include <QString>

namespace paco
{


template <typename T>
class PacketType_T;

/**
 * @brief The PacketType class stores the packet type of a package.
 */
class PacketType
{
public:

    /**
     * @brief PacketType default constructor.
     */
    PacketType()
    {
        mPacketString = "<PacketType not specified>";
        mPacketDescription = "";
    }

protected:

    /**
     * @brief mPacketString contains the type of the packet.
     */
    QString mPacketString;

    /**
     * @brief mPacketDescription contains the description of the packet.
     */
    QString mPacketDescription;

public:

    /**
     * @brief toString returns the type id string of the packet.
     * @return the type id string of the packet.
     */
    virtual QString toString() const
    {
        return mPacketString;
    }

    /**
     * @brief equals checks if this packet type id matches another packet type id.
     * @param other the packet to compare with.
     * @return true if the packets type id's match.
     */
    bool equals(PacketType& other)
    {
        return other.toString().compare(this->toString()) == 0;
    }


    /**
     * @brief equals equals<type>() checks if this packet type id matches another type id using angle brackets.
     * @return true if the packet matches a type.
     */
    template <class T>
    bool equals()
    {
        PacketType_T<T> other = PacketType_T<T>();
        PacketType* otherBase = dynamic_cast<PacketType*>(&other);

        return otherBase->toString().compare(this->toString()) == 0;
    }

};


/**
 * @brief operator == checks if two packets have the same type id.
 * @param lhs
 * @param rhs
 * @return true if both packet type id's match.
 */
inline bool operator==(const PacketType& lhs, const PacketType& rhs)
{
    return lhs.toString().compare(rhs.toString()) == 0;
}

/**
 * @brief operator != checks if two packets have NOT the same type id.
 * @param lhs
 * @param rhs
 * @return true if both packet type id's do NOT match.
 */
inline bool operator!=(const PacketType& lhs, const PacketType& rhs)
{ return !(lhs == rhs); }




/**
 * @brief The PacketType_T template class stores the packet type of a package.
 */
template <typename T>
class PacketType_T : public PacketType
{
public:
    /**
     * @brief PacketType_T is the default constructor.
     * @param description is the description of the packet.
     */
    PacketType_T(QString description = "")
    {
        mPacketString = toStringHelper();
        mPacketDescription = description;
    }

public:

    /**
     * @brief toString returns the packet type id string.
     * @return the packet type id string.
     */
    virtual QString toString() const
    {
        return mPacketString;
    }

private:

    /**
     * @brief toStringHelper is a helper function to create a human readable type id string from the type id.
     * @return a human readable type id string from the type id.
     */
    QString toStringHelper()
    {
        QString typeIdString;


        const std::type_info  &ti = typeid(T);

#if _WIN32 || _WIN64
#if !__GNUC__
        //msvc
        typeIdString = /*"Packet_T*/"<" + QString(ti.name()) + ">";
#endif
#endif

// Check GCC, then use demangle to format the type name nicely.
#if __GNUC__
        int     status;
        char   *realname;

        realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
        typeIdString = QString("<" + QString(realname) + ">");
        free(realname);
#endif


        typeIdString.replace("<class ", "<");
        typeIdString.replace(" ", "");
        typeIdString.replace(",classstd::allocator", "");
        typeIdString.replace(",std::allocator", "");

        /*
        if(typeIdString.startsWith("<"))
        {
            typeIdString = typeIdString.remove(0,1);
            typeIdString = typeIdString.remove(typeIdString.length()-1,1);
        }*/

        return typeIdString;
    }
};

}


#endif // PACKET_TYPE_H
