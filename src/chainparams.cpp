// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2015-2017 The ALQO developers
// Copyright (c) 2018 The COFFEECOIN developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>


using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"
#include "test/bignum.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
	(0, uint256("0x"))
        (0, uint256("0x"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    0, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    0        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xca;
        pchMessageStart[1] = 0xdb;
        pchMessageStart[2] = 0xf7;
        pchMessageStart[3] = 0x19;
        vAlertPubKey = ParseHex("04d20b9320f2eb1808b01116dd5ada62edff469b609e894c15d5dc73e64f052887a800ba197a63c897ddc6729737dfc0db3ef7a138a47770ad96981d898141cf23");
        nDefaultPort = 4366;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // COFFEECOIN starting difficulty is 1 / 2^12  //20
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // COFFEECOIN: 1 day
        nTargetSpacing = 2 * 60;  // COFFEECOIN: 2 minute
        nLastPOWBlock = 262800;
        nMaturity = 100;
        nMasternodeCountDrift = 20;
	    nMasternodeColleteralLimxDev = 1000; //Params().MasternodeColleteralLimxDev()
        nModifierUpdateBlock = 1; // we use the version 2 for dmd
        nMaxMoneyOut = 88000000 * COIN;

        const char* pszTimestamp = "This is genesis block for CoffeeCoin network";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040cf8d9264dcec329e2108b58c174b6dd9da4e3af5263905548a6cbfabb6d7d79d6d55953bfce87bae7dede18cf0c4a8829733e9414edd38305ebaa74293b7d84") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1535108400;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 4663919;

        hashGenesisBlock = genesis.GetHash();
        if(genesis.GetHash() != uint256S("0x"))
        {
            bool fNegative;
            bool fOverflow;
            uint256 bigNum;
            //printf("Searching for genesis block...\n");
            bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
            while(uint256(genesis.GetHash()) > bigNum)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    //printf("NONCE WRAPPED, incrementing time");
                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                    ++genesis.nTime;
                }
                if (genesis.nNonce % 10000 == 0)
                {
                    //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                }
            }
            printf("Mainnet block.nBits = %u \n", genesis.nBits);
            printf("Mainnet block.nTime = %u \n", genesis.nTime);
            printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
            printf("Mainnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        }
        std::cout << std::string("Finished calculating Testnet Genesis Block:\n");

        //assert(hashGenesisBlock == uint256("0x00000cc30945ca7b20bbeabbefd9bf6d682f1c9e797f32690f4974454ba5f7cb"));
        //assert(genesis.hashMerkleRoot == uint256("0x01cfab24acf686cbf9794f98906162f0b5905e93d3d3ff0d9090b53e6973c05f"));

        vSeeds.push_back(CDNSSeedData("seed1", "seed1.coffeecoin.store"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.coffeecoin.store"));
        vSeeds.push_back(CDNSSeedData("seed3", "seed3.coffeecoin.store"));

        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 109 	0x8000006d
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04cd66b21f8273bf8c6ddd2b97fe5f830271637ceb82bc6269b010fef7090aa05657b78178f3605c11abd8567dbca949012e864a5132b355dcfa4d33346eaeee1f";
        strDarksendPoolDummyAddress = "GNEZjVTQHjff8BoWvUhGMSg9KZA7QNEwUm";
        nStartMasternodePayments = 1524781746; //11/10/2017 @ 12:00am (UTC)
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xa3;
        pchMessageStart[1] = 0x44;
        pchMessageStart[2] = 0xf5;
        pchMessageStart[3] = 0xe4;
        vAlertPubKey = ParseHex("04e1d34432d3a5803312f33cf20686b1801835ea8efb18ad77d50b5ec1ce5ca73de65c36c082ca5c51880f8ffb5a296d8dc780f1dc02f22eba4dde1816a6a54b75");
        nDefaultPort = 4367;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // COFFEECOIN: 1 day
        nTargetSpacing = 2 * 60;  // COFFEECOIN: 1 minute
        nLastPOWBlock = 262800;
        nMaturity = 15;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 88000000 * COIN;

        genesis.nTime = 1535108600;
        genesis.nNonce = 5202609;

        hashGenesisBlock = genesis.GetHash();
        if(genesis.GetHash() != uint256S("0x"))
        {
            bool fNegative;
            bool fOverflow;
            uint256 bigNum;
            //printf("Searching for genesis block...\n");
            bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
            while(uint256(genesis.GetHash()) > bigNum)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    //printf("NONCE WRAPPED, incrementing time");
                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                    ++genesis.nTime;
                }
                if (genesis.nNonce % 10000 == 0)
                {
                    //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                }
            }
            printf("Mainnet block.nBits = %u \n", genesis.nBits);
            printf("Mainnet block.nTime = %u \n", genesis.nTime);
            printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
            printf("Mainnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        }
        std::cout << std::string("Finished calculating Testnet Genesis Block:\n");/***/

        //assert(hashGenesisBlock == uint256("0x00000647e92bd8283b2b9cfff8157692140c97addac0110cb744ad9afdb7a80b"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("seed1", "seed1.coffeecoin.store"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.coffeecoin.store"));
        vSeeds.push_back(CDNSSeedData("seed3", "seed3.coffeecoin.store"));
		
	    base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 84);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 195);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x81)(0x62)(0xa1).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x81)(0x59)(0x38).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x01)(0x01)(0x02).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));


        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "049e53e687fdafd78fd42d730fad0e7ea1819396176a2cb85d7a76fa4559cdbd2c2f05330a6f5cbadb44a6c1d324f167e679e9f3e95d9d5649761a3e7f59bf4500";
        strDarksendPoolDummyAddress = "";
        nStartMasternodePayments = 1562531573;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x14;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0x95;
        pchMessageStart[3] = 0x64;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // COFFEECOIN: 1 day
        nTargetSpacing = 1 * 60;        // COFFEECOIN: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1535108900;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 4663919;
		
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 4368;
        if(genesis.GetHash() != uint256S("0x"))
        {
            bool fNegative;
            bool fOverflow;
            uint256 bigNum;
            //printf("Searching for genesis block...\n");
            bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
            while(uint256(genesis.GetHash()) > bigNum)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    //printf("NONCE WRAPPED, incrementing time");
                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                    ++genesis.nTime;
                }
                if (genesis.nNonce % 10000 == 0)
                {
                    //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                }
            }
            printf("Mainnet block.nBits = %u \n", genesis.nBits);
            printf("Mainnet block.nTime = %u \n", genesis.nTime);
            printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
            printf("Mainnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
            printf("Mainnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
        }
        std::cout << std::string("Finished calculating Testnet Genesis Block:\n");
        /***/

       //assert(hashGenesisBlock == uint256("0x00000cc30945ca7b20bbeabbefd9bf6d682f1c9e797f32690f4974454ba5f7cb"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
