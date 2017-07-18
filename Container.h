// Package Container library (paco)

// Copyright 2017 (c) Thomas Pollok (tom dot pollok at gmail dot com)

// https://github.com/tompollok/paco

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTAINER_H
#define CONTAINER_H

#include <algorithm>

#include "Packet.h"
#include "PacketType.h"
#include "Specification.h"


namespace paco
{

/**
 * @brief The Container class is a datastructure that can carry data with varying data types.
 * The container is organized in as a list so that you can use typical methods like append and insert to add types and
 * remove or clear to remove a single or all objects from the container. All data that is stored is enveloped in a package that
 * allows to check the typeid of an object at runtime.
 *
 * When interacting with a container you always have to specify the object type in the angle brackets.
 *
 * Here are some examples of how to add objects to the container:
 *
 * - to add an <int>,         container.append<int>(value);
 * - to add an <std::string>, container.append<std::string>(value);
 * - to add an <QVector*>, container.append<QVector*>(value);
 * - etc.
 *
 * The type in the angle brackets has to match the type of the value, or otherwise the compiler will not proceed.
 */

class Container
{
public:

    /**
     * @brief Container default constructor.
     */
    Container()
    {

    }

    /**
     * @brief ~Container destructor.
     */
    ~Container()
    {
        this->clear();
    }

private:

    /**
     * template_type_must_be_specified_when_calling_append is a helper to force the programmer
     * to use angle brackets <type> in append<type>(value). If not specified the compiler will not be able to compile.
     */
    template <typename T>
    struct template_type_must_be_specified_when_calling_append
    {
        using type = T;
    };

public:

    /**
     * @brief size
     * @return the number of elements in the container.
     */
    int size()
    {
        return mPackets.size();
    }

    /**
     * @brief append the method for appending objects into the container.
     * The object is automatically wrapped into a package.
     */
    template <class T>
    void append(typename template_type_must_be_specified_when_calling_append<T>::type value)
    {
        mPackets.push_back(new paco::Packet_T<T>(value));
    }

    /**
     * @brief insert the method for inserting objects into the container into specific locations.
     * The object is automatically wrapped into a package.
     * @param index the desired index.
     */
    template <class T>
    void insert(int index, typename template_type_must_be_specified_when_calling_append<T>::type value)
    {
        mPackets.insert(mPackets.begin() + index, new paco::Packet_T<T>(value));
    }
    
    /**
     * @brief removeAt removes an object from the container at a certain index position.
     * The container class is not responsible for your memory management of the objects.
     * @param index the index of the object that has to be removed.
     */
    void removeAt(int index)
    {
        if(index >= 0 && index < mPackets.size())
        {
            paco::Packet* packet = mPackets.at(index);
            mPackets.erase(mPackets.begin()+index);
            delete packet;
        }
    }

    /**
     * @brief removeAt removes an object from the container at a certain index position.
     * @param index the index of the object that has to be removed.
     */
    void clear()
    {
        for(int i = 0; i < mPackets.size(); i++)
        {
            paco::Packet* packet = mPackets.at(i);
            delete packet;
        }

        mPackets.clear();
    }

    /**
     * @brief replace replaces an object from the container at a certain index position.
     * @param index the index of the object that has to be replaced.
     */
    template <class T>
    void replace(int i, const T value)
    {
        paco::Packet* old = mPackets.at(i);
        std::replace(mPackets.begin(), mPackets.end(), old, dynamic_cast<Packet*>(new paco::Packet_T<T>(value)));
        delete old;
    }

    /**
     * @brief at
     * @param index the index of the package with the object.
     * @return returns the package carrying the object at position i.
     */
    paco::Packet* at(int index)
    {
        return mPackets.at(index);
    }


    /**
     * @brief getSpecification returns a specification of the objects of this container.
     * @return the current specification of the container.
     */
    Specification getSpecification()
    {
        Specification specification;

        for(int i = 0; i < mPackets.size(); i++)
        {
            specification.append_friend_class_only(mPackets.at(i)->packetType());
        }

        return specification;
    }

private:

    /**
     * @brief mPackets the actual container.
     */
    std::vector<paco::Packet*> mPackets;

};

}
#endif // CONTAINER_H
