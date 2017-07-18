#include <QCoreApplication>

#include "PacketType.h"
#include "Packet.h"
#include "Container.h"
#include "Specification.h"

#include <QVector>

#include <algorithm>
#include <functional>



void process(paco::Container& container)
{

    //creating some data
    QList<int>* intList = new QList<int>();
    intList->append(42);
    intList->append(1999);

    QList<QString>* stringList = new QList<QString>();
    stringList->append("paco");
    stringList->append("template");
    stringList->append("library");
    stringList->append("is");
    stringList->append("amazing");
    stringList->append(".");


    //append a QList<int>*
    container.append<QList<int>*>(intList);

    //append a QList<QString>*
    container.append<QList<QString>*>(stringList);

    //append a std::vector<int>*
    container.append<std::vector<int>*>(new std::vector<int>(50));

    //append an int
    container.append<int>(42);

    //append a lambda function with two parameters lambdafunction(int x, double y)
    container.append<std::function<void (int, double )>>([](int x, double y){ std::cout << "x: '" << x << "' y: '" << y << "'" << std::endl;});

}




int main(int argc, char *argv[])
{

    //create a container
    paco::Container container;

    //process container
    process(container);

    //get container specification after processing
    paco::Specification containerSpecification = container.getSpecification();


    //print the specification of the container after processing it
    std::cout << "printing container specification" << std::endl;
    for(int i = 0; i < containerSpecification.size(); i++)
    {
        paco::PacketType p = containerSpecification.at(i);
        std::cout << p.toString().toStdString() << std::endl;
    }
    std::cout << std::endl << std::endl;


    //print out all container element types
    std::cout << "Listing all container element types:" << std::endl;
    for(int i = 0; i < container.size(); i++)
    {
        std::cout << container.at(i)->packetType().toString().toStdString() << std::endl;
    }
    std::cout << std::endl << std::endl;


    //extracting data from container
    //in practice you dont need a for loop, but you can access the container as follows:
    //container.at(0)->get<your_type>();
    std::cout << "extracting data from elements" << std::endl;
    for(int i = 0; i < container.size(); i++)
    {
        paco::Packet* p = container.at(i);
        std::cout << "at index " << i << " there is obviously a " << p->packetType().toString().toStdString() << std::endl;

        //checking for QList<int>*
        if(p->packetType().equals<QList<int>*>())
        {
            std::cout << "found a QList<int>* at " << i << std::endl;

            QList<int>* list = p->get<QList<int>*>();
            //QList<double>* list2 = p->get<QList<double>*>(); // this would throw an exception, because double is wrong

            std::cout << "printing elements " << std::endl;
            for(int j : *list)
            {
                std::cout << "'" << j << "'" << std::endl;
            }
        }


        //checking for QList<QString>*
        if(p->packetType().equals<QList<QString>*>())
        {
            std::cout << "found a QList<QString>* at " << i << std::endl;

            QList<QString>* list = p->get<QList<QString>*>();

            std::cout << "printing elements " << std::endl;
            for(QString j : *list)
            {
                std::cout << "'" << j.toStdString() << "'" << std::endl;
            }
        }


        //checking for an int
        if(p->packetType().equals<int>())
        {
            std::cout << "found a <int> at " << i << std::endl;
            int value = p->get<int>();
            std::cout << "printing int value: '" << value << "'" << std::endl;
        }


        //checking for a pointer to a vector of int
        if(p->packetType().equals<std::vector<int>*>())
        {
            std::cout << "found a <std::vector<int>*> at " << i << std::endl;
        }


        //checking for an int
        if(p->packetType().equals<std::function<void (int, double)>>())
        {
            std::function<void (int, double)> lamdaFunction = p->get<std::function<void (int, double)>>();

            lamdaFunction(42, 49.999);
        }


        std::cout << std::endl;
    }

    return 1;
}
