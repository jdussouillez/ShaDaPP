#include <iostream>

#include <shadapp/data/Serializer.h>
#include <shadapp/fs/Folder.h>
#include <shadapp/fs/FileSplitter.h>
#include <shadapp/data/Hash.h>
#include <algorithm>

#include <QFileInfo>

#include <shadapp/Logger.h>

typedef std::map<std::string, std::string> TStrFileInfoMap;

namespace shadapp {

    namespace fs {

        Folder::Folder(std::string id, std::string path) : id(id), path(path) {
            onWatch = false;
        }

        Folder::Folder(std::string id) : Folder(id, "") {
            onWatch = false;
        }

        Folder::Folder(std::vector<uint8_t>& bytes) {
            uint32_t idLength = shadapp::data::Serializer::deserializeInt32(bytes);
            id = shadapp::data::Serializer::deserializeString(bytes, idLength);
            uint32_t nbDevices = shadapp::data::Serializer::deserializeInt32(bytes);
            onWatch = false;
            for (uint32_t i = 0; i < nbDevices; i++) {
                Device* d = new Device(bytes);
                addDevice(d);
                // TODO: Fix memory leak (= delete d)
            }
        }

        Folder::~Folder() {
            devices.clear();
        }

        void Folder::createFileWatcher(std::string foldersPath) {
            fileWatcher = new shadapp::fs::FileWatcher(foldersPath + path, 10);
            connect(fileWatcher, SIGNAL(newFileSignal(std::string)), this, SLOT(slotFileWatcherCreate(std::string)));
            connect(fileWatcher, SIGNAL(deletedFileSignal(std::string)), this, SLOT(slotFileWatcherDelete(std::string)));
            connect(fileWatcher, SIGNAL(modifiedFileSignal(std::string)), this, SLOT(slotFileWatcherModify(std::string)));
            //fileWatcher->run();
            onWatch = true;
            fileWatcher->start();
            Logger::debug("onWatch %s", (foldersPath + path).c_str());
        }

        void Folder::startFileWatcher() {
            onWatch = true;
            Logger::debug("onWatch %d", onWatch);
        }

        void Folder::stopFileWatcher() {
            onWatch = false;
            Logger::debug("onWatch %d", onWatch);
        }

        bool Folder::operator==(const Folder& f1) {
            return this->getId().compare(f1.getId()) == 0;
        }

        void Folder::addDevice(Device* device) {
            devices.push_back(device);
        }

        void Folder::addFileInfo(FileInfo& fileInfo) {
            bool exist = false;
            for (auto fi : fileInfos) {
                if (fileInfo.getName().compare(fi.getName()) == 0) {
                    Logger::error("FileInfo is already in this folder");
                    exist = true;
                }
            }
            if (!exist) {
                fileInfos.push_back(fileInfo);
            }
        }

        void Folder::replaceFileInfo(std::string name, FileInfo& fileInfo) {
            int pos = 0;
            for (auto fi : fileInfos) {
                if (fi.getName().compare(name) == 0) {
                    fileInfos.erase(fileInfos.begin() + pos);
                }
                pos++;
            }
            fileInfos.push_back(fileInfo);
        }

        std::string Folder::getId() const {
            return id;
        }

        std::string Folder::getPath() const {
            return path;
        }

        std::vector<Device*> Folder::getDevices() const {
            return devices;
        }

        std::vector<shadapp::fs::FileInfo> Folder::getFileInfos() {
            return fileInfos;
        }

        void Folder::setPath(std::string path) {
            this->path = path;
        }

        std::vector<uint8_t> Folder::serialize() const {
            std::vector<uint8_t> bytes;
            shadapp::data::Serializer::serializeInt32(bytes, id.length());
            shadapp::data::Serializer::serializeString(bytes, id);
            shadapp::data::Serializer::serializeInt32(bytes, devices.size());
            for (auto d : devices) {
                std::vector<uint8_t> deviceBytes = d->serialize();
                bytes.insert(bytes.end(), deviceBytes.begin(), deviceBytes.end());
            }
            return bytes;
        }

        void Folder::slotFileWatcherCreate(std::string name) {
            if (onWatch) {
                std::string fileName = name.substr(name.find_last_of("/\\") + 1);
                bool exist = false;
                for (auto &fileInfo : fileInfos) {
                    if (fileInfo.getName().compare(fileName) == 0) {
                        exist = true;
                    }
                }
                if (!exist) {
                    Logger::info("[FileWatcher] Create \"%s\"", fileName.c_str());
                    //
                    QFileInfo fileInfo(QString(name.c_str()));
                    if (fileInfo.isFile()) {
                        shadapp::fs::FileSplitter splitter(fileInfo.absoluteFilePath().toStdString());
                        std::vector<shadapp::fs::BlockInfo> blocks;
                        uint64_t offset = 0;
                        for (unsigned int i = 0; i < splitter.getNbBlocks(); i++) {
                            std::vector<char> block = splitter.getBlock(offset, MAX_BLOCK_SIZE);
                            std::string hash;
                            shadapp::data::Hash256::hash(reinterpret_cast<uint8_t*> (&block[0]), block.size(), hash);
                            shadapp::fs::BlockInfo blockInfo(hash, block.size());
                            blocks.push_back(blockInfo);
                            offset += MAX_BLOCK_SIZE;
                        }
                        qint64 time = fileInfo.lastModified().toMSecsSinceEpoch() / 1000;
                        shadapp::fs::FileInfo* fInfo = new shadapp::fs::FileInfo(fileInfo.fileName().toStdString(),
                                0, (uint64_t) time, 0, 0, blocks);
                        this->addFileInfo(*fInfo);
                        emit signalFileModify(this, fInfo);
                    }
                    //                    
                }
            }
        }

        void Folder::slotFileWatcherDelete(std::string name) {
            if (onWatch) {
                std::string fileName = name.substr(name.find_last_of("/\\") + 1);
                Logger::info("[FileWatcher] Delete \"%s\"", fileName.c_str());
                for (auto &fileInfo : fileInfos) {
                    if (fileInfo.getName().compare(fileName) == 0) {
                        fileInfo.setDeleted(true);
                        emit signalFileModify(this, &fileInfo);
                    }
                }
            }
        }

        void Folder::slotFileWatcherModify(std::string name) {
            Logger::debug("onWatch %d", onWatch);
            Logger::debug("ALLO");
            if (onWatch) {
                std::string fileName = name.substr(name.find_last_of("/\\") + 1);
                Logger::info("[FileWatcher] Modify \"%s\"", fileName.c_str());
                for (auto &fileInfo : fileInfos) {
                    if (fileInfo.getName().compare(fileName) == 0) {
                        fileInfo.increaseVersion(name);
                        emit signalFileModify(this, &fileInfo);
                    }
                }
            }
        }

    }
}
