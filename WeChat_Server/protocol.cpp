#include "protocol.h"

PDU* mkpdu(uint uimsglen)
{
    uint uipdulen = sizeof (PDU) + uimsglen;
    PDU* pdu = (PDU*)malloc(uipdulen);

    if(pdu == NULL)
    {
        exit(1);
    }

    memset(pdu, 0, uipdulen);
    pdu->uiPDUlen = uipdulen;
    pdu->uiMsgLen = uimsglen;
    return pdu;
}
