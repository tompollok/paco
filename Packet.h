// Package Container library (paco)

// Copyright 2017 (c) Thomas Pollok (tom dot pollok at gmail dot com)

// https://github.com/tompollok/paco

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PACKET_H
#define PACKET_H

#include <QString>
#include <stdexcept>
#include <typeinfo>

#include "PacketType.h"

// FIXME: add category loggging or remove debug outputs

namespace paco
{

class PacketType;
class Packet;

template <class T>
T packet_cast(Packet* packet);

template <typename T>
class Packet_T;


/**
 * @brief The Packet class represents a abstract single packet that stores data of type T.
 */

class Packet
{
public:

    /**
     * @brief getType returns the type of the container.
     * @return the packet type.
     */

    virtual paco::PacketType packetType() = 0;

    /**
     * @brief get<T> returns the data of this packet in the specified template type T.
     * @return the data of the packet as T. If T and data do not match then a std::bad_cast is thrown.
     */
    template <class T>
    T get() //throw(std::bad_cast)
    {
        try
        {

            paco::Packet_T<T>* packet = paco::packet_cast<paco::Packet_T<T>*>(this);

            //if(packet != nullptr)
            {
                return packet->data();
            }

        }
        catch(std::bad_cast& e)
        {
            throw e;
        }

        //return nullptr;
    }

};

/**
 * @brief The Packet_T templace class is a template packet that can store any kind of data.
 */

template <typename T>
class Packet_T : public Packet
{
public:

    /**
     * @brief Packet_T is the constructor with the templaet parameter.
     * @param data the object stored in this package.
     */
    Packet_T(T data)
    {
        mData = data;
    }

private:
    /**
     * @brief mData is the data of this package.
     */
    T mData;

public:

    /**
     * @brief data returns the data of this package.
     * @return the data of this package.
     */
    T data()
    {
        return mData;
    }

    /**
     * @brief packetType returns the package type of the package.
     * @return the package type of the package.
     */
    virtual paco::PacketType packetType()
    {

        return paco::PacketType_T<T>();
    }

};

/**
 * @brief packet_cast casts the packet into a package of data type T.
 * Throws an std::bad_cast exception in case the desired package type T
 * does not match the packets actual data type.
 * @param the package type of the package.
 */
template <class T>
T packet_cast(Packet* packet)
{
    T castedPacket = dynamic_cast<T>(packet);

    if(packet != nullptr && castedPacket == nullptr)
    {
        QString typeNameStringDst = typeid(T).name();
        typeNameStringDst.append("#");
        typeNameStringDst.replace("class " , "");
        typeNameStringDst.replace("paco::Packet_T" , "");
        typeNameStringDst.replace("*#" , "");
        typeNameStringDst.replace(" " , "");

        QString typeNameStringSrc = packet->packetType().toString();

        //assert(typeNameStringSrc.toStdString() == typeNameStringDst.toStdString());

        QString errorMessage = "bad cast error: casting Packet data from '" + typeNameStringSrc + "' to '" + typeNameStringDst + "' not possible.";

        std::cout << "#################################################################################################################" << std::endl;
        std::cout << errorMessage.toStdString() << std::endl;
        std::cout << "#################################################################################################################" << std::endl;

        throw std::bad_cast();//errorMessage.toStdString().c_str());

    }

    return castedPacket;
}


}

#endif // PACKET_H
