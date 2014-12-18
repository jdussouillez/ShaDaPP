#include <shadapp/Core.h>
#include <shadapp/protocol/IndexMessage.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/protocol/RequestMessage.h>

#include <iostream>
#include <fstream>

#include <shadapp/Network.h>

namespace shadapp {

    namespace protocol {

        IndexMessage::IndexMessage(
                std::bitset<4> version,
                std::string folder,
                std::vector<shadapp::fs::FileInfo> files)
        : AbstractIndexMessage(Type::INDEX, version, folder, files) {
        }

        IndexMessage::IndexMessage(std::vector<uint8_t>& bytes)
        : AbstractIndexMessage(bytes) {
        }

        void IndexMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            //identification du folder
            shadapp::fs::Folder* indexFolder;
            for (auto &folder : lp.getConfig()->getFolders()) {
                if (folder->getId().compare(this->getFolder()) == 0) {
                    indexFolder = folder;
                    indexFolder->setPath(folder->getId()+ "/" );
                }
            }
            
            QString path((lp.getConfig()->getFoldersPath() + indexFolder->getPath()).c_str());
            QDir dir(path);
            if (dir.mkdir(path)) { //false if dir already exists, return true if dir was created with success                
                //create files
                for (auto &fileInfo : getFiles()) {
                    uint64_t fileSize = 0;
                    for(auto &block : fileInfo.getBlocks()){
                        std::cout << "taille du block" << block.getSize() << std::endl;                 
                        fileSize += block.getSize();
                    }
                    //Put 0 in file
                    std::ofstream file(lp.getConfig()->getFoldersPath() + indexFolder->getPath() + fileInfo.getName());
                    for(uint64_t i = 0; i < fileSize; i++){
                        file << "0";
                    }
                    
                }
                for (auto &fileInfo : getFiles()) {
                    uint64_t offset = 0;
                    for(auto &block : fileInfo.getBlocks()){
                        std::cout << "taille du block" << block.getSize() << std::endl;
                        shadapp::protocol::RequestMessage requestMessage(
                            *(lp.getConfig()->getVersion()),
                            getFolder(),
                            fileInfo.getName(),
                            offset,
                            block.getSize());
                            lp.getNetwork()->send(device.getSocket(), requestMessage);
                        offset += block.getSize();
                    }
                    
                }
            } else {
                //parcourrir les filesInfo
                //Parcourrir les fichiers locaux
                //Si match => comparer les hashs
                //Si hash diff?rents => download
                //Sinon => ne rien faire

                //Si pas de match => ajouter aux fichiers ? telecharger
            }

//            std::cout << "Folder : " << indexFolder->getId() << std::endl;
//            for (auto &file : this->getFiles()) {
//                std::cout << "file : " << file.getName() << std::endl;
//                std::cout << "nbBlock : " << file.getBlocks().size() << std::endl;
//                for (auto &block : file.getBlocks()) {
//                    std::cout << "block hash: " << block.getHash() << std::endl;
//                    std::cout << "block size : " << block.getSize() << std::endl;
//                }
//            }
        }
    }
}
