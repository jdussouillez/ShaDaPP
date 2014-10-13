#ifndef BLOCKINFO_H
#define	BLOCKINFO_H

namespace shadapp {

    class BlockInfo {
    private:
        unsigned int size;
        char* hash;

    public:
        explicit BlockInfo(unsigned int size);
        virtual ~BlockInfo();

        unsigned int getSize() const;
        char* getHash() const;


    };
}



#endif	// BLOCKINFO_H 
