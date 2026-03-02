#include "stm32l476xx.h"
#include "usart.h"
#include "aes.h"


// AES-128 Key (16 bytes)
static const uint8_t AES_KEY[16] = {
    0x2b, 0x7e, 0x15, 0x16,
    0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88,
    0x09, 0xcf, 0x4f, 0x3c
};

// IV (16 bytes) — doit changer à chaque message en production
static const uint8_t AES_IV[16] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f
};

// --- PKCS#7 Padding: remplit le buffer jusqu'au multiple de 16 ---
// Retourne la nouvelle taille
uint16_t pad_pkcs7(uint8_t *buf, uint16_t data_len, uint16_t buf_size) {
    uint8_t pad = 16 - (data_len % 16);  // nombre de bytes à ajouter
    if (data_len + pad > buf_size) return 0; // sécurité
    for (uint8_t i = 0; i < pad; i++)
        buf[data_len + i] = pad;             // valeur du padding = nombre de bytes ajoutés
    return data_len + pad;
}


int main(void) {
    // GPIO PA5 (LED)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void)RCC->AHB2ENR;
    GPIOA->MODER &= ~(3U << 10);
    GPIOA->MODER |=  (1U << 10);

    __enable_irq();
    USART3_Init();

    // La chaine a chiffrer 
    const char *message = "Jonatan apprend AES CBC";

    // Buffer avec marge pour le padding (max 16 bytes ajoutés)
    uint8_t buffer[32];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, message, strlen(message));

    // Appliquer le padding PKCS#7
    uint16_t padded_len = pad_pkcs7(buffer, strlen(message), sizeof(buffer));

    // Chiffrer avec AES-CBC
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, AES_KEY, AES_IV);
    AES_CBC_encrypt_buffer(&ctx, buffer, padded_len);

    while (1) {
        // Cette boucle me permet de verifier que le code est bien fonctionnel
        // lorsque la led clignote continuellement, je sais que tout va bien 
        // Blink LED
        GPIOA->BSRR = (1U << 5);
        for (volatile uint32_t i = 0; i < 200000; i++) __NOP();
        GPIOA->BSRR = (1U << (5 + 16));
        for (volatile uint32_t i = 0; i < 200000; i++) __NOP();

        // Envoi du  message chiffré en HEX
        USART3_SendString("Encrypted: ");
        USART3_SendHex(buffer, padded_len);
    }
}
