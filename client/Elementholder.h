#ifndef __ELEMENTHOLDER_H__
#define __ELEMENTHOLDER_H__

#include "../global/psi_utils.h"
#include "../server/Keyholder.h"
#include "client.h"

using namespace std;
using namespace NTL;

class Elementholder{
    public:
    int id;
    int* elements;
    int num_elements;
    ZZ alpha;
    ZZ alpha_inv;
    pcs_public_key *pk;
    pcs_private_key *vk;
    hcs_random *hr;

    Elementholder(){}
    Elementholder(int __id, int bitsize);
    Elementholder(int __id, int* __elements, int __num_elements, int bitsize);

    void Scheme1_Round1(ZZ *h_x_alpha, ZZ *g_alpha, Context public_context, int __X);
    Scheme1_Round2_send Scheme1_Round2(Context context, Scheme1_Round1_receive out);
    void Scheme1_Final(ZZ &secret_share, mpz_t __mpz_secret);

    Share get_share_1(Context context, int __X, client* elem_holder, int num_bins);

    Scheme0_send Scheme0_Round1(Context public_context, int __X, int id);
    void Scheme0_Final(ZZ *secret_share, Context public_context, ZZ secret_share_alpha);

    Share get_share_0(Context context, int __X, client* elem_holder, int num_bins);
};

#endif