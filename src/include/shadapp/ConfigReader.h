#ifndef CONFIGREADER_H
#define	CONFIGREADER_H

#include <string>

#include <QtCore/QString>
#include <QtXml/QXmlDefaultHandler>

#include <shadapp/ClientConfig.h>

#define DISABLE_UNUSED_WARN(var) \
    do { \
        (void) var; \
    } while (false)

namespace shadapp {

    class ConfigReader : public QXmlDefaultHandler {
    private:
        ClientConfig clientConfig;
        Folder* folder;
        bool inPort;
        bool inName;

    public:
        ConfigReader();

        ClientConfig getClientConfig() const;

        static ClientConfig parse(std::string fileName, std::string XsdFile);

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

#endif	// CONFIGREADER_H
