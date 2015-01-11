#include <shadapp/protocol/ResponseMessage.h>
#include <shadapp/protocol/RequestMessage.h>
#include <shadapp/data/Serializer.h>
#include <shadapp/LocalPeer.h>
#include <shadapp/fs/RequestedBlock.h>
#include <shadapp/data/Hash.h>

#include <iostream>
#include <fstream> 

namespace shadapp {

    namespace protocol {

        ResponseMessage::ResponseMessage(std::bitset<4> version, std::bitset<12> id, std::string data)
        : AbstractMessage(id, version, Type::RESPONSE, false),
        data(data.substr(0, MAX_BLOCK_SIZE)) {
        }

        ResponseMessage::ResponseMessage(std::vector<uint8_t>& bytes)
        : AbstractMessage(bytes) {
            uint32_t length = shadapp::data::Serializer::deserializeInt32(bytes);
            data = shadapp::data::Serializer::deserializeString(bytes, length);
        }

        std::string ResponseMessage::getData() const {
            return data;
        }

        std::vector<uint8_t> ResponseMessage::serialize() const {
            std::vector<uint8_t> bytes = AbstractMessage::serialize();
            shadapp::data::Serializer::serializeInt32(bytes, data.length());
            shadapp::data::Serializer::serializeString(bytes, data);
            // Set the message's length
            shadapp::data::Serializer::serializeInt32(bytes, bytes.size(), 4);
            return bytes;
        }

        void ResponseMessage::executeAction(shadapp::fs::Device& device, shadapp::LocalPeer& lp) const {
            shadapp::fs::RequestedBlock* rqBlock = lp.getRequestedBlocks()[getId().to_ulong()];
            std::string hash;
            shadapp::data::Hash256::hash(static_cast<const uint8_t*> ((uint8_t*) (getData().c_str())), getData().size(), hash);
            if (hash.compare(rqBlock->getHash()) == 0) {
                std::string pathh = lp.getConfig()->getFoldersPath() + rqBlock->getFolder()->getPath();
                std::fstream outfile;
                outfile.open(lp.getConfig()->getFoldersPath() + rqBlock->getFolder()->getPath() + rqBlock->getFileName());
                if (outfile.is_open()) {
                    outfile.seekp(rqBlock->getOffset());
                    outfile.write(getData().c_str(), rqBlock->getSize());
                    outfile.close();
                    if (rqBlock->decreaseDownloadBlockRemaning() == 0) {                        
                        rqBlock->getFileInfo()->setLocalVersion(rqBlock->getFileInfo()->getVersion());
                        Logger::info("File fully download at version : %d", rqBlock->getFileInfo()->getLocalVersion());
                    }

                } else {
                    shadapp::Logger::error("Cannot open file");
                }
            }else{
                Logger::error("Hashes are differents => re-send the request");
                std::bitset<12> newId = lp.generateMessageId();
                rqBlock->setId(newId);
                shadapp::protocol::RequestMessage requestMessage(
                    *(lp.getConfig()->getVersion()),
                    newId,
                    rqBlock->getFolder()->getId(),
                    rqBlock->getFileName(),
                    rqBlock->getOffset(),
                    rqBlock->getSize());
            }

        }
    }
}
