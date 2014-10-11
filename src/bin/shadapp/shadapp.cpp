#include <iostream>
#include <getopt.h>
#include <string>

#include <QCoreApplication>
#include <QtCore/QDir>

#include <shadapp/ClientConfig.h>
#include <shadapp/ConfigReader.h>

#include "config.h"

#define APPNAME "shadapp"

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
    QCoreApplication a(argc, argv);
    
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
    shadapp::ClientConfig config;
    try {
        config = shadapp::ConfigReader::parse(std::string(configFile), "src/resources/shadapp/configSchema.xsd");
        std::cout << "Version = " << config.getVersion() << std::endl;
        std::cout << "Port = " << config.getPort() << std::endl;
        std::cout << "Folders :" << std::endl;
        for (auto f : config.getFolders()) {
            std::cout << "\tID = " << f.getId() << std::endl;
            std::cout << "\tPath = " << f.getPath() << std::endl;
            for (auto d : f.getDevices()) {
                std::cout << "\t\tID = " << d.getId() << std::endl;
                std::cout << "\t\tFlag = " << d.getFlag() << std::endl;
                std::cout << "\t\tMaxLocalVersion = " << d.getMaxLocalVersion() << std::endl;
                std::cout << std::endl;
            }
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
