#ifndef __INET_TCPFIT_H
#define __INET_TCPFIT_H

#include "inet/transportlayer/tcp/Tcp.h"
#include "inet/common/INETDefs.h"
//#include <algorithm>
#include "inet/transportlayer/tcp/flavours/TcpBaseAlg.h"
#include "inet/transportlayer/tcp/flavours/TcpSegmentTransmitInfoList.h"

namespace inet {
namespace tcp {

class INET_API TCPFitStateVariables : public TcpBaseAlgStateVariables
{

public:
    TCPFitStateVariables();
    ~TCPFitStateVariables();

    virtual std::string info() const override;
    virtual std::string detailedInfo() const override;

    TcpSegmentTransmitInfoList regions;

    double alpha; /* Additive increase */
    double beta; /* Muliplicative decrease */
    double nValue; /* tcp-fit n parameter */
    double cwnd_cnt; /* Congestion window counter. */
    simtime_t upd_interval; /* interval used for updating N */
    simtime_t w_RTTmin; /* min RTT */

    simtime_t w_RTTmax; /* max RTT */

    simtime_t epoch_start; /* when an update occurred  */
    simtime_t update_epoch; /* update interval for N */

    simtime_t RTT_cnt; /* rtt counter */
    simtime_t avgRTT; /* average Round Trip time; */
    simtime_t w_lastAckTime;    // last received ack time
    uint32 ACK_cnt; /* ACK counter */
    uint32 ssthresh; /* < slow start threshold */
    std::vector<uint32>ssthreshVector;
    std::vector<uint32>cwndVector;

private:
};

class INET_API TCPFit : public TcpBaseAlg
{
protected:
    cOutVector *nValueVector = new cOutVector("N Value");
    cOutVector *ssthreshVector = new cOutVector("Ssthresh Value");
    cOutVector *cwndVector = new cOutVector("cwnd Value");
    TCPFitStateVariables *& state;    // alias to TCPFit algorithm's 'state'

    /** Create and return a TCPFitStateVariables object. */
    virtual TcpStateVariables *createStateVariables() override
    {
        return new TCPFitStateVariables();
    }

    /** Utility function to recalculate ssthresh */
    virtual void recalculateSlowStartThreshold();
    /** Redefine what should happen on retransmission */
    virtual void processRexmitTimer(TcpEventCode& event) override;

public:
    /** Ctor */
    TCPFit();
    /* Dtor */
    ~TCPFit();

    /** Redefine what should happen when data got acked, to add congestion window management */
    virtual void receivedDataAck(uint32 firstSeqAcked) override;

    /** Redefine what should happen when dupAck was received, to add congestion window management */
    virtual void receivedDuplicateAck() override;

    /** Called after we send data */
    virtual void dataSent(uint32 fromseq) override;

    virtual void segmentRetransmitted(uint32 fromseq, uint32 toseq) override;

    virtual void tcpFitUpdateN();

private:
    simtime_t currentTime; // current time in simulation

    virtual void tcpFitReset();
};
}
}
#endif
