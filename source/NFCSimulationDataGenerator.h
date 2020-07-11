#ifndef NFC_SIMULATION_DATA_GENERATOR
#define NFC_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class NFCAnalyzerSettings;

class NFCSimulationDataGenerator
{
public:
	NFCSimulationDataGenerator();
	~NFCSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, NFCAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	NFCAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptorGroup	mNFCSimulationChannels;
	SimulationChannelDescriptor *mTxSimulationData;
	SimulationChannelDescriptor *mRxSimulationData;

};
#endif //NFC_SIMULATION_DATA_GENERATOR
