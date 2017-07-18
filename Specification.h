// Package Container library (paco)

// Copyright 2017 (c) Thomas Pollok (tom dot pollok at gmail dot com)

// https://github.com/tompollok/paco

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTAINERSPECIFICATION_H
#define CONTAINERSPECIFICATION_H

#include "Packet.h"
#include "PacketType.h"
#include <vector>

namespace paco
{


/**
 * @brief The Specification class reflects a type list specification using different types.
 * A container itself has a specification which can be matched with an expected specification.
 */
class Specification
{
    friend class Container;

public:
    /**
     * @brief Specification default constructor.
     */
    Specification()
    {

    }

    /**
     * @brief ~Specification default constructor.
     */
    ~Specification()
    {

    }

    /**
     * @brief size returns the number of elements in this specification.
     * @return the number of elements in this specification.
     */
    int size()
    {
        return mPacketTypes.size();
    }

    /**
     * @brief append<T> appends a type id to the specification.
     * @param description is the description of the object that is expected here.
     */
    template <class type>
    void append(QString description = "")
    {

        mPacketTypes.push_back(paco::PacketType_T<type>(description));
    }

    /**
     * @brief insert<T> insert a packet type id to the specification at a certain index location.
     * @param description is the description of the object that is expected here.
     */
    template <class type>
    void insert(int index, QString description = "")
    {

        mPacketTypes.insert(mPacketTypes.begin() + index, paco::PacketType_T<type>(description));
    }

    /**
     * @brief removeAt removes a packet type at a certain index location.
     * @param index
     */
    void removeAt(int index)
    {
        if(index >= 0 && index < mPacketTypes.size())
        {
            mPacketTypes.erase(mPacketTypes.begin() + index);
        }
    }

    /**
     * @brief at returns the packet type at a certain index.
     * @param index is the index location.
     * @return the packet type at a certain index.
     */
    paco::PacketType at(int index)
    {
        return mPacketTypes.at(index);
    }

private:
    void append_friend_class_only(paco::PacketType packetType)
    {

        mPacketTypes.push_back(packetType);
    }

private:

    /**
     * @brief mPacketTypes the list containing the packet types.
     */
    std::vector<paco::PacketType> mPacketTypes;

};

}

#endif // CONTAINERSPECIFICATION_H
