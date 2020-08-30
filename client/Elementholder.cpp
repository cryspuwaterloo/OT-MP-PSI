#include "Elementholder.h"

using namespace std;
using namespace NTL;

Elementholder::Elementholder(int __id, int bitsize){
    id=__id;
    pk = pcs_init_public_key();
    vk = pcs_init_private_key();
    hr = hcs_init_random();
    pcs_generate_key_pair(pk, vk, hr, 2 * bitsize + 80);
}

Elementholder::Elementholder(int __id, int* __elements, int __num_elements, int bitsize){
    id=__id;
    elements=__elements;
    num_elements=__num_elements;
    pk = pcs_init_public_key();
    vk = pcs_init_private_key();
    hr = hcs_init_random();
    pcs_generate_key_pair(pk, vk, hr, 2 * bitsize + 80);
}

void Elementholder::Scheme1_Round1(ZZ *h_x_alpha, ZZ *g_alpha, Context public_context, int __X){
    ZZ X = ZZ(__X);
    ZZ_p::init(public_context.p);
    ZZ_p g_p;
    conv(g_p, public_context.g);
    ZZ_p h_x = hash_XX(X, public_context.p);
    {
        ZZ_pPush push(public_context.p-1);
        ZZ_p __temp = random_ZZ_p();
        while  (GCD(rep(__temp), public_context.p-1) > 1) {
            __temp = random_ZZ_p();
        }
        alpha = rep(__temp);
        __temp = 1 / __temp;
        alpha_inv = rep(__temp);
    }

    *h_x_alpha = rep(NTL::power(h_x, alpha));
    *g_alpha = rep(NTL::power(g_p, alpha));

    return;
}

Scheme1_Round2_send Elementholder::Scheme1_Round2(Context context, Scheme1_Round1_receive out){

    ZZ_p::init(context.p);
    int t = context.t;
    Scheme1_Round2_send output(t, pk, id);

    ZZ_p masked_coefficients[t-1];
    for (int i=0; i < t-1; i++){
        masked_coefficients[i] = NTL::power(conv<ZZ_p>(out.masked_coefficients_alpha[i]), alpha_inv);
    }

    for (int i=0;i<t-1;i++){
        ZZ_p_to_mpz_t(output.mpz_coefficients[i], masked_coefficients[i]);
    }

    pcs_encrypt(pk, hr, output.mpz_secret, output.mpz_secret);
    for (int i=0;i<t-1;i++){
        pcs_encrypt(pk, hr, output.mpz_coefficients[i], output.mpz_coefficients[i]);
    }
    return output;
}

void Elementholder::Scheme1_Final(ZZ &secret_share, mpz_t __mpz_secret){
    pcs_decrypt(vk, __mpz_secret, __mpz_secret);	
    mpz_t_to_ZZ(secret_share, __mpz_secret);
}

Share Elementholder::get_share_1(Context context, int __X, client* elem_holder, int num_bins){
    
    ZZ_p::init(context.p);
    string result;
    
    //Round 1 
    ZZ h_x_alpha, g_alpha;
    Scheme1_Round1(&h_x_alpha, &g_alpha, context, __X);

    Scheme1_Round1_send to_send_round_1;
    to_send_round_1.h_x_alpha = h_x_alpha;
    to_send_round_1.g_alpha = g_alpha;
    result = elem_holder->send_to_server("S1_R1", to_send_round_1.toString());
    Scheme1_Round1_receive out_round1 = Scheme1_Round1_receive(result);
    
    //Round 2
    Scheme1_Round2_send to_send_round_2 = Scheme1_Round2(context, out_round1);
    result = elem_holder->send_to_server("S1_R2", to_send_round_2.toString());
    Scheme1_Round2_receive out_round2 = Scheme1_Round2_receive(result);

    ZZ secret_share;
    Scheme1_Final(secret_share, out_round2.mpz_secret);

    return Share(
        ZZ(id),
        rep(hash_XX(ZZ(__X), ZZ(num_bins))),
        ZZ(secret_share)
    );
}

Scheme0_send Elementholder::Scheme0_Round1(Context public_context, int __X, int id){
    ZZ p = public_context.p, q = public_context.q;
	int t = public_context.t;
	ZZ_p::init(p);
	ZZ_p h_x = hash_XX(ZZ(__X), p);
	{
		ZZ_pPush push(p-1);
		ZZ_p __temp = random_ZZ_p();
		while  (GCD(rep(__temp), ZZ(p-1)) > 1) {
			__temp = random_ZZ_p();
		}
		alpha = rep(__temp);
		__temp = 1 / __temp;
		alpha_inv = rep(__temp);
	}
    Scheme0_send output;
	output.h_x_alpha = rep(NTL::power(h_x, alpha));
    output.id = id;
    return output;
}

void Elementholder::Scheme0_Final(ZZ *secret_share, Context public_context, ZZ secret_share_alpha){
	*secret_share = rep(NTL::power(conv<ZZ_p>(secret_share_alpha), alpha_inv));
}

Share Elementholder::get_share_0(Context context, int __X, client* elem_holder, int num_bins){
    
    ZZ_p::init(context.p);
    string result;

    Scheme0_send to_send = Scheme0_Round1(context, __X, id);
    result = elem_holder->send_to_server("S0", to_send.toString());
    Scheme0_receive received = Scheme0_receive(result);
    
    ZZ secret_share;
    Scheme0_Final(&secret_share, context, received.secret_share_alpha);
    return Share(
        ZZ(id),
        rep(hash_XX(ZZ(__X), ZZ(num_bins))),
        ZZ(secret_share)
    );
}