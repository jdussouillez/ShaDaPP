#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>
#include <vector>

#include <QtCore/QString>
#include <QtXml/QXmlDefaultHandler>

#include <shadapp/config/PeerConfig.h>

#define DISABLE_UNUSED_WARN(var) ((void) var)

namespace shadapp {

    namespace config {

        class ConfigReader : public QXmlDefaultHandler {
        private:
            PeerConfig* peerConfig;
            shadapp::fs::Folder* folder;
            shadapp::fs::Device* device;
            bool inID;
            bool inPort;
            bool inName;
            bool inScanPeriod;
            bool inDevice;
            bool inAddress;
            bool inFolder;

            static bool toBool(std::string const& s);

        public:
            ConfigReader();
            ConfigReader(const ConfigReader& other);

            PeerConfig* getPeerConfig() const;

            static PeerConfig* parse(std::string fileName, std::string XsdFile);

            virtual bool startElement(
                    const QString& namespaceURI,
                    const QString& localName,
                    const QString& qName,
                    const QXmlAttributes& att);
            virtual bool characters(const QString& str);
            virtual bool endElement(
                    const QString& namespaceURI,
                    const QString& localName,
                    const QString& qName);

            ConfigReader& operator=(const ConfigReader& other);
        };
    }
}

#endif	// CONFIGREADER_H
