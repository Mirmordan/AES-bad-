#pragma once
#include <array>

class aes_block {
    std::array<std::array<unsigned char,4>,4> state;           //Поля
    std::array<std::array<unsigned char,44>,4> key_schelude;    
    static const std::array<unsigned char, 256> sub_box;
    static const std::array<unsigned char, 256> inverse_sub_box;
    public:
    aes_block(std::array<unsigned char,16> cipher_key);       //Конструкторы
    aes_block();
    aes_block(const aes_block& other);

    void Expand_Key(std::array<unsigned char,16> cipher_key);  //Методы шифрования
    void AddRoundKey(int key_number);
    private:
    void SubBytes();
    void InvSubBytes();
    void MixCollums();
    void InvMixCollums();
    void ShiftRows();
    void InvShiftRows();
    public:
    void BytesToMatrix(std::array<unsigned char,16> input);
    const std::array<unsigned char,16> MatrixToBytes();
    void Encrypt();
    void Decrypt();
    private:
    const unsigned char GF8_mul_by_2(unsigned char number); //Арифметика в полях Галуа
    const unsigned char GF8_mul_by_3(unsigned char number);
    const unsigned char GF8_mul_by_9(unsigned char number);
    const unsigned char GF8_mul_by_b(unsigned char number);
    const unsigned char GF8_mul_by_d(unsigned char number);
    const unsigned char GF8_mul_by_e(unsigned char number);
    public:
    const void PrintState();                  //Методы логирования
    const void PrintSchelude();

    public:
    aes_block& operator=(const aes_block& other) ;       //Перегрузка операторов
    aes_block& operator^=(const aes_block& other);
    aes_block operator^(const aes_block& other)const;
};

