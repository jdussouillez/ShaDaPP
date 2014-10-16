#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>

#include <QtCore/QString>
#include <QtXml/QXmlDefaultHandler>

#include <shadapp/config/PeerConfig.h>

#define DISABLE_UNUSED_WARN(var) \
    do { \
        (void) var; \
    } while (false)

namespace shadapp {

    namespace config {

        class ConfigReader : public QXmlDefaultHandler {
        private:
            PeerConfig peerConfig;
            shadapp::fs::Folder* folder;
            bool inPort;
            bool inName;

        public:
            ConfigReader();

            PeerConfig getClientConfig() const;

            static PeerConfig parse(std::string fileName, std::string XsdFile);

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
        };
    }
}

#endif	// CONFIGREADER_H
