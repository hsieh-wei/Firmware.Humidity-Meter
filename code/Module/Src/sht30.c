#include "sht30.h"

// ---- internal helpers ---- 

// ----------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------
int pc_link_init(PC_LINK_HANDLE *h)
{
    if (!h || !h->huart || !h->rx_buf || h->rx_len == 0) {
        return PC_LINK_ERROR;
    }

    h->busy_tx = 0;
    //turn off Half-Transfer IT
    pc_link_disable_dma_half_it(h->huart);
    return PC_LINK_SUCCESS;
}

// ----------------------------------------------------------------------------
// HAL Weak Callback re define 
// ----------------------------------------------------------------------------