#ifndef NFC_ANALYZER_H
#define NFC_ANALYZER_H

#include <Analyzer.h>
#include "NFCAnalyzerResults.h"
#include "NFCSimulationDataGenerator.h"

class NFCAnalyzerSettings;
class ANALYZER_EXPORT NFCAnalyzer : public Analyzer2
{
public:
	NFCAnalyzer();
	virtual ~NFCAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< NFCAnalyzerSettings > mSettings;
	std::auto_ptr< NFCAnalyzerResults > mResults;
	AnalyzerChannelData* mRxData;
	AnalyzerChannelData* mTxData;

	NFCSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //NFC_ANALYZER_H
