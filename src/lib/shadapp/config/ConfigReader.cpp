#include <cstdlib>

#include <QtCore/QFile>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

#include <shadapp/config/ConfigReader.h>

namespace shadapp {

    namespace config {

        ConfigReader::ConfigReader()
        : folder(nullptr), device(nullptr), inPort(false), inName(false),
                inScanPeriod(false), inDevice(false), inAddress(false), inFolder(false) {
            peerConfig = new PeerConfig();
        }

        ConfigReader::ConfigReader(const ConfigReader& other) : QXmlDefaultHandler() {
            operator=(other);
        }

        PeerConfig* ConfigReader::getPeerConfig() const {
            return peerConfig;
        }

        PeerConfig* ConfigReader::parse(std::string fileName, std::string XsdFile) {
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
            QXmlInputSource* inputSource;
            QXmlSimpleReader reader;
            ConfigReader configReader;
            inputSource = new QXmlInputSource(&file);
            reader.setContentHandler(&configReader);
            if (!reader.parse(inputSource)) {
                throw std::runtime_error("Error: error while parsing \"" + fileName + "\"");
            }
            delete inputSource;
            return configReader.getPeerConfig();
        }

        bool ConfigReader::startElement(
                const QString& namespaceURI,
                const QString& localName,
                const QString& qName,
                const QXmlAttributes& att) {
            DISABLE_UNUSED_WARN(namespaceURI);
            DISABLE_UNUSED_WARN(localName);
            if (qName.compare("shadapp") == 0) {
                peerConfig->setVersion(att.value("version").toStdString());
            } else if (qName.compare("port") == 0) {
                inPort = true;
            } else if (qName.compare("name") == 0) {
                inName = true;
            } else if (qName.compare("scan-period") == 0) {
                inScanPeriod = true;
            } else if (qName.compare("address") == 0) {
                inAddress = true;
            } else if (qName.compare("device") == 0) {
                std::string id = att.value("id").toStdString();
                if (!inFolder) {
                    inDevice = true;
                    bool trust = toBool(att.value("trusted").toStdString());
                    bool readOnly = toBool(att.value("read-only").toStdString());
                    bool introducer = toBool(att.value("introducer").toStdString());
                    device = new shadapp::fs::Device(id);
                    device->setTrusted(trust);
                    device->setReadOnly(readOnly);
                    device->setIntroducer(introducer);
                    // TODO: get priority attribute
                } else {
                    for (auto d : peerConfig->getDevices()) {
                        if (d->getId().compare(id) == 0) {
                            // Add pointer to device in folder
                            folder->addDevice(d);
                            return true;
                        }
                    }
                    throw std::runtime_error("The device \"" + id + "\" does not exist");
                }
            } else if (qName.compare("folder") == 0) {
                inFolder = true;
                folder = new shadapp::fs::Folder(att.value("id").toStdString(), att.value("path").toStdString());
            } else if (qName.compare("option") == 0) {
                peerConfig->addOption(att.value("name").toStdString(), att.value("value").toStdString());
            }
            return true;
        }

        bool ConfigReader::characters(const QString& str) {
            std::string s = str.toStdString();
            if (inPort) {
                unsigned short port = std::atoi(s.c_str());
                peerConfig->setPort(port);
                inPort = false;
            } else if (inName) {
                if (inDevice) {
                    device->setName(s);
                } else {
                    peerConfig->setName(s);
                }
                inName = false;
            } else if (inScanPeriod) {
                unsigned int scanPeriod = std::atoi(s.c_str());
                peerConfig->setScanPeriod(scanPeriod);
                inScanPeriod = false;
            } else if (inAddress) {
                int splitIndex = s.find(":");
                device->setAddress(s.substr(0, splitIndex));
                device->setPort(std::atoi(s.substr(splitIndex + 1).c_str()));
                inAddress = false;
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
                peerConfig->addFolder(*folder);
                delete folder;
                inFolder = false;
            } else if (qName.compare("device") == 0) {
                if (!inFolder) {
                    peerConfig->addDevice(device);
                    inDevice = false;
                }
            }
            return true;
        }

        bool ConfigReader::toBool(std::string const& s) {
            if (s.empty()) {
                return false;
            }
            return s.compare("1") == 0 || s.compare("true") == 0;
        }

        ConfigReader& ConfigReader::operator=(const ConfigReader& other) {
            if (this != &other) {
                peerConfig = other.peerConfig;
                folder = other.folder;
                device = other.device;
                inPort = other.inPort;
                inName = other.inName;
                inScanPeriod = other.inScanPeriod;
                inDevice = other.inDevice;
                inAddress = other.inAddress;
                inFolder = other.inFolder;
            }
            return *this;
        }
    }
}
