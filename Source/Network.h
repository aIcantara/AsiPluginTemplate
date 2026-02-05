#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <RakHook/rakhook.hpp>
#include <RakNet/StringCompressor.h>

#include <string>

class CNetwork
{
public:
    CNetwork() = default;
    ~CNetwork();

    bool Initialize();

private:
    bool OnReceiveRPC(unsigned char& id, RakNet::BitStream* pBitStream);
    bool OnSendRPC(int& id, RakNet::BitStream* pBitStream, PacketPriority& priority, PacketReliability& reliability, char& ordChannel, bool& shTimestamp);
    bool OnReceivePacket(Packet* pPacket);
    bool OnSendPacket(RakNet::BitStream* pBitStream, PacketPriority& priority, PacketReliability& reliability, char& ordChannel);

    template <typename T>
    bool ReadWithSize(RakNet::BitStream& bitStream, std::string& string);

    template <typename T>
    void WriteWithSize(RakNet::BitStream& bitStream, std::string_view string);
};

#endif // __NETWORK_H__