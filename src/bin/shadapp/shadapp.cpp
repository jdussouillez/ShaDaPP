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
    QCoreApplication app(argc, argv);

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
            std::cout << "\tID = " << f.getId() << std::endl;
            std::cout << "\tPath = " << f.getPath() << std::endl;
            for (auto d : f.getDevices()) {
                std::cout << "\t\tID = " << d->getId() << std::endl;
                std::cout << "\t\tName = " << d->getName() << "  (mem addr: " << d << ")" << std::endl;
                std::cout << std::endl;
            }
        }

        //TODO: "remove this"
        //        shadapp::protocol::ClusterConfigMessage conf(
        //                *config->getVersion(),
        //                config->getName(),
        //                "blabla",
        //                config->getFolders(),
        //                config->getOptions());
        //        std::vector<uint8_t> bytes = conf.serialize();
        //        shadapp::protocol::ClusterConfigMessage conf2(bytes);
        //        std::cout << "OK FINAL" << std::endl;
        //        std::cout << conf.getVersion() << " - " << conf2.getVersion() << std::endl;
        //        std::cout << conf.getType() << " - " << conf2.getType() << std::endl;
        //        std::cout << conf.getClientName() << " - " << conf2.getClientName() << std::endl;
        //        std::cout << conf.getClientVersion() << " - " << conf2.getClientVersion() << std::endl;
        //        std::cout << conf.getId() << " - " << conf2.getId() << std::endl;
        //        std::cout << conf.getFolders().size() << " - " << conf2.getFolders().size() << std::endl;
        //        for (unsigned int i = 0; i < conf.getFolders().size(); i++) {
        //            std::cout << std::endl;
        //            std::cout << conf.getFolders().at(i).getId() << " - " << conf2.getFolders().at(i).getId() << std::endl;
        //            std::cout << conf.getFolders().at(i).getPath() << " - " << conf2.getFolders().at(i).getPath() << std::endl;
        //            for (unsigned int j = 0; j < conf.getFolders().at(i).getDevices().size(); j++) {
        //                std::cout << conf.getFolders().at(i).getDevices().at(j)->getId() << " - " << conf2.getFolders().at(i).getDevices().at(j)->getId() << std::endl;
        //                std::cout << conf.getFolders().at(i).getDevices().at(j)->getMaxLocalVersion() << " - " << conf2.getFolders().at(i).getDevices().at(j)->getMaxLocalVersion() << std::endl;
        //                std::cout << std::endl;
        //            }
        //        }
        //        std::cout << std::endl;
        //        std::cout << conf.getOptions().size() << " - " << conf2.getOptions().size() << std::endl;
        //        for (auto it : conf.getOptions()) {
        //            std::cout << it.first << " = " << it.second << std::endl;
        //        }
        //        for (auto it : conf2.getOptions()) {
        //            std::cout << it.first << " = " << it.second << std::endl;
        //        }
        //TODO: end "remove this"


        // TODO: remove "this"
        //        std::bitset<4> v;
        //        v.set(0);
        //        std::vector<shadapp::fs::BlockInfo> blocks;
        //        blocks.push_back(shadapp::fs::BlockInfo("my_data_1", 9));
        //        blocks.push_back(shadapp::fs::BlockInfo("my_data_2", 9));
        //        std::vector<shadapp::fs::FileInfo> files;
        //        files.push_back(shadapp::fs::FileInfo("name1", 42, blocks));
        //        shadapp::protocol::IndexMessage idx1(v, "my_folder", files);
        //        std::vector<uint8_t> bytes2 = idx1.serialize();
        //        shadapp::protocol::IndexMessage idx2(bytes2);
        //        std::cout << "\n\n";
        //        std::cout << idx1.getType() << " = " << idx2.getType() << std::endl;
        //        std::cout << idx1.getFolder() << " = " << idx2.getFolder() << std::endl;
        //        std::cout << idx1.getFiles().size() << " = " << idx2.getFiles().size() << std::endl;
        //        std::cout << "--> File info : " << std::endl;
        //        for (unsigned int i2 = 0; i2 < idx1.getFiles().size(); i2++) {
        //            std::cout << idx1.getFiles().at(i2).getName() << " = " << idx1.getFiles().at(i2).getName() << std::endl;
        //            std::cout << "-----> Blocks : " << std::endl;
        //            for (unsigned int i3 = 0; i3 < idx1.getFiles().at(i2).getBlocks().size(); i3++) {
        //                std::cout << idx1.getFiles().at(i2).getBlocks().at(i3).getSize() << " = " << idx2.getFiles().at(i2).getBlocks().at(i3).getSize() << std::endl;
        //                std::cout << idx1.getFiles().at(i2).getBlocks().at(i3).getHash() << " = " << idx2.getFiles().at(i2).getBlocks().at(i3).getHash() << std::endl;
        //            }
        //        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // TODO: remove this
    shadapp::Logger::setLevel(shadapp::Logger::Level::ALL);
    shadapp::Logger::enableColors(true);
    shadapp::Logger::debug("foo");
    shadapp::Logger::debug("test va_list : %d, 0x%x, %s", 10, 255, "abcd");
    shadapp::Logger::info("bar");
    shadapp::Logger::success("Successfully sent !");
    shadapp::Logger::warn("baz");

    //std::logic_error ex("err msg");
    //shadapp::Logger::error("qux", &ex);
    //shadapp::Logger::fatal("norf");

    //tests Maxime
    //shadapp::LocalPeer localPeer(0, std::string(configFile));
    //shadapp::Network localPeer(0, std::string(configFile));
    //localPeer.start();
    //fin test Maxime
    //delete config;
    // TODO: remove this
    //    shadapp::fs::FileSplitter splitter("test/config.xml");
    //    std::cout << "Blocks = " << splitter.getNbBlocks() << std::endl;
    //    std::vector<char> block = splitter.getBlock(0, 100);
    //    for (std::vector<char>::size_type i = 0; i != block.size(); i++) {
    //        std::cout << block.at(i);
    //    }

    // TODO: remove this test (HASH)
    std::string hash;
    uint8_t input[] = {"abc"};
    shadapp::data::Hash256::hash(input, 3, hash);
    std::cout << hash << std::endl;

//    try {
//        app.exec();
//    } catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
}
