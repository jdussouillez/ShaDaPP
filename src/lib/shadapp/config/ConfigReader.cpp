#include <iostream>
#include <stdint.h>
#include <string>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

#include <shadapp/config/ConfigReader.h>

namespace shadapp {

    namespace config {

        ConfigReader::ConfigReader() : folder(NULL), inPort(false), inName(false) {
        }

        PeerConfig ConfigReader::getClientConfig() const {
            return peerConfig;
        }

        PeerConfig ConfigReader::parse(std::string fileName, std::string XsdFile) {
            // Check if the file exists
            QFile file(QString(fileName.c_str()));
            if (!file.exists()) {
                throw std::runtime_error("Error: cannot access \"" + fileName + "\"");
            }

            // Check if the file is valid using a XSD file
            QXmlSchema schema;
            schema.load(QUrl::fromLocalFile(QString(XsdFile.c_str())));
            if (!schema.isValid()) {
                throw std::runtime_error("Error: The XML Schema file is not valid");
            }
            file.open(QIODevice::ReadOnly);
            QXmlSchemaValidator validator(schema);
            if (!validator.validate(&file, QUrl::fromLocalFile(QString(fileName.c_str())))) {
                throw std::runtime_error("Error: XML file not valid");
            }
            file.close();

            // Parse the file
            QXmlInputSource *inputSource;
            QXmlSimpleReader reader;
            ConfigReader configReader;
            inputSource = new QXmlInputSource(&file);
            reader.setContentHandler(&configReader);
            if (!reader.parse(inputSource)) {
                throw std::runtime_error("Error: error while parsing \"" + fileName + "\"");
            }
            delete inputSource;
            return configReader.getClientConfig();
        }

        bool ConfigReader::startElement(
                const QString& namespaceURI,
                const QString& localName,
                const QString& qName,
                const QXmlAttributes& att) {
            DISABLE_UNUSED_WARN(namespaceURI);
            DISABLE_UNUSED_WARN(localName);
            if (qName.compare("shadapp") == 0) {
                peerConfig.setVersion(att.value("version").toStdString());
            } else if (qName.compare("port") == 0) {
                inPort = true;
            } else if (qName.compare("name") == 0) {
                inName = true;
            } else if (qName.compare("folder") == 0) {
                folder = new shadapp::fs::Folder(att.value("id").toStdString(), att.value("path").toStdString());
            } else if (qName.compare("device") == 0) {
                std::string id = att.value("id").toStdString();
                uint32_t flag = std::stoul(att.value("flag").toStdString(), nullptr, 10);
                uint64_t maxLocalVersion = std::stoull(att.value("maxLocalVersion").toStdString(), nullptr, 10);
                folder->addDevice(shadapp::fs::Device(id, flag, maxLocalVersion));
            }
            return true;
        }

        bool ConfigReader::characters(const QString& str) {
            bool ok = true;
            if (inPort) {
                unsigned short port = str.toUShort(&ok);
                if (!ok) {
                    throw new std::runtime_error("Error: port is invalid");
                }
                peerConfig.setPort(port);
                inPort = false;
            } else if (inName) {
                peerConfig.setName(str.toStdString());
                inName = false;
            }
            return true;
        }

        bool ConfigReader::endElement(
                const QString& namespaceURI,
                const QString& localName,
                const QString& qName) {
            DISABLE_UNUSED_WARN(namespaceURI);
            DISABLE_UNUSED_WARN(localName);
            if (qName.compare("folder") == 0) {
                peerConfig.addFolder(*folder);
                delete folder;
            }
            return true;
        }
    }
}
