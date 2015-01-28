#include <bitset>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <string>

#include <QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QThread>

#include <shadapp/Logger.h>
#include <shadapp/config/PeerConfig.h>
#include <shadapp/config/ConfigReader.h>
#include <shadapp/data/Hash.h>
#include <shadapp/fs/FileSplitter.h>
#include <shadapp/fs/FileWatcher.h>
#include <shadapp/protocol/ClusterConfigMessage.h>
#include <shadapp/protocol/CloseMessage.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/protocol/IndexUpdateMessage.h>
#include <shadapp/protocol/PingMessage.h>
#include <shadapp/protocol/PongMessage.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/protocol/ResponseMessage.h>

#include <shadapp/Network.h>

#include "config.h"
#include <shadapp/SafeApplication.h>

static void printUsage(void) {
    std::cout << "Usage: " << APPNAME << " -c CONFIG_FILE" << std::endl;
}

static void printVersion(void) {
    std::cout << APPNAME << " " << VERSION << std::endl;
    std::cout << "\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n";
    std::cout << "This is free software: you are free to change and redistribute it.\n";
    std::cout << "There is NO WARRANTY, to the extent permitted by law.\n";
}

static int parseArguments(int argc, char** argv, bool* usage, bool* version, char** configFile) {
    int opt = 0;
    static struct option longOptions[] = {
        {"config", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(argc, argv, "c:Vh", longOptions, NULL)) != -1) {
        switch (opt) {
            case 'c':
                *configFile = optarg;
                break;
            case 'V':
                *version = true;
                return 0;
            case 'h':
                *usage = true;
                return 0;
            default:
                *usage = true;
                return 1;
        }
    }
    return (*usage || *version || configFile != NULL) ? 0 : 1;
}

int main(int argc, char **argv) {
    // We have to start a QCoreApplication to use the XSD validation.
    // Otherwise, there is an error "QEventLoop: Cannot be used without QApplication".
    shadapp::SafeApplication app(argc, argv);

    // Parse arguments
    bool usage = false, version = false;
    char* configFile = NULL;
    int errorCode = parseArguments(argc, argv, &usage, &version, &configFile);
    if (usage) {
        printUsage();
        return errorCode;
    }
    if (version) {
        printVersion();
        return errorCode;
    }
    if (configFile == NULL) {
        std::cerr << "Error: You must specify the configuration file.\n";
        std::cerr << "Try '" << APPNAME << " --help' for more information.\n";
        return 1;
    }

    std::cout << "Using file: " << configFile << std::endl;
    shadapp::config::PeerConfig* config = nullptr;
    try {
        config = shadapp::config::ConfigReader::parse(std::string(configFile), "src/resources/shadapp/configSchema.xsd");
        std::cout << "Version = " << config->getVersion()->to_string() << std::endl;
        std::cout << "Id = " << config->getID() << std::endl;
        std::cout << "Port = " << config->getPort() << std::endl;
        std::cout << "Folders :" << std::endl;
        for (auto f : config->getFolders()) {
            std::cout << "\tID = " << f->getId() << std::endl;
            std::cout << "\tPath = " << f->getPath() << std::endl;
            for (auto d : f->getDevices()) {
                std::cout << "\t\tID = " << d->getId() << std::endl;
                std::cout << "\t\tName = " << d->getName() << std::endl;
                std::cout << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    shadapp::Logger::setLevel(shadapp::Logger::Level::ALL);
    shadapp::Logger::enableColors(true);

    //tests Maxime
    shadapp::LocalPeer localPeer(0, std::string(configFile));
    //shadapp::Network localPeer(0, std::string(configFile));
    localPeer.start();
    //fin test Maxime

    try {
        app.exec();
        //SafeApplication::SafeApplication(int &argc, char *argv[]);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    delete config;
    return 0;
}
