#include "NFCAnalyzer.h"
#include "NFCAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

NFCAnalyzer::NFCAnalyzer()
:	Analyzer2(),
	mSettings( new NFCAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

NFCAnalyzer::~NFCAnalyzer()
{
	KillThread();
}

void NFCAnalyzer::SetupResults()
{
	mResults.reset( new NFCAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mTxChannel );
}

void NFCAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mTxData = GetAnalyzerChannelData( mSettings->mTxChannel );
	mRxData = GetAnalyzerChannelData( mSettings->mRxChannel );

	if( mTxData->GetBitState() == BIT_LOW )
		mTxData->AdvanceToNextEdge();

	U32 samples_per_bit = mSampleRateHz / mSettings->mBitRate;
	U32 samples_to_first_center_of_first_data_bit = U32( 1.5 * double( mSampleRateHz ) / double( mSettings->mBitRate ) );

	for( ; ; )
	{
		U8 data = 0;
		U8 mask = 1 << 7;

		mTxData->AdvanceToNextEdge(); //falling edge -- beginning of the start bit

		U64 starting_sample = mTxData->GetSampleNumber();

		mTxData->Advance( samples_to_first_center_of_first_data_bit );

		for( U32 i=0; i<8; i++ )
		{
			//let's put a dot exactly where we sample this bit:
			mResults->AddMarker( mTxData->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mTxChannel );

			if( mTxData->GetBitState() == BIT_HIGH )
				data |= mask;

			mTxData->Advance( samples_per_bit );

			mask = mask >> 1;
		}


		//we have a byte to save.
		Frame frame;
		frame.mData1 = data;
		frame.mFlags = 0;
		frame.mStartingSampleInclusive = starting_sample;
		frame.mEndingSampleInclusive = mTxData->GetSampleNumber();

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
}

bool NFCAnalyzer::NeedsRerun()
{
	return false;
}

U32 NFCAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 NFCAnalyzer::GetMinimumSampleRateHz()
{
	return mSettings->mBitRate * 4;
}

const char* NFCAnalyzer::GetAnalyzerName() const
{
	return "NFC protocol analyzer";
}

const char* GetAnalyzerName()
{
	return "NFC protocol analyzer";
}

Analyzer* CreateAnalyzer()
{
	return new NFCAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
