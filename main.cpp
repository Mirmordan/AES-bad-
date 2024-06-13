#include "AES.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main(){
cout <<"Enter input filename: "<<endl;
string input_filename="plaintext.txt";        //Открытие файлов
cin >> input_filename;
string output_filename="o"+input_filename;
ifstream input_file(input_filename,ios::binary);
ofstream output_file(output_filename,ios::binary|ios::trunc);
if(!input_file) cout<<"no input file"<<endl;
if(!output_file) cout<<"no output file"<<endl;
array<unsigned char,3> bom = {0xEF, 0xBB, 0xBF},input_bom;
output_file.write(reinterpret_cast<char*>(bom.data()), 3);
input_file.read(reinterpret_cast<char*>(input_bom.data()), 3);
if(input_bom!=bom)input_file.seekg(0,ios_base::beg);




ifstream keys("keys.txt",ios::binary);                    //Чтение ключа
if(!keys) cout<<"no keys file";
array <unsigned char,16> cipher_key,initial_vector;
if(!keys.read(reinterpret_cast<char*>(cipher_key.data()), 16)) cout<<"No key";
if(!keys.read(reinterpret_cast<char*>(initial_vector.data()), 16)) cout<<"No init_vector";
cout<<"Cipher key: ";
for(auto i:cipher_key) cout<<i;
cout<<endl<<"Cipher key: ";
for(auto i:initial_vector) cout<<i;
cout<<endl;

bool is_decrypt=false;                          //Режим работы CFB
char aes_mode;
cout<<"'E'ncryption or 'd'ecryption?"<<endl;
cin >> aes_mode;
if(aes_mode=='d' or aes_mode=='D') is_decrypt=true;


aes_block previous_block(cipher_key),block=previous_block;           //CFB AES всех блоков с первого по без последнего
previous_block.BytesToMatrix(initial_vector);
array <unsigned char,16> byte_input,byte_output;
while(input_file.read(reinterpret_cast<char*>(byte_input.data()), 16)){
    block.BytesToMatrix(byte_input);
    previous_block.Encrypt();
    previous_block ^= block;
    byte_output = previous_block.MatrixToBytes();
    output_file.write(reinterpret_cast<char*>(byte_output.data()), 16);
    if(is_decrypt) previous_block=block;
}
if(input_file.gcount()!=0){                                     //Обработка последнего блока
    for(int i=input_file.gcount();i<16;i++) byte_input[i]=0;
    block.BytesToMatrix(byte_input);
    previous_block.Encrypt();
    previous_block ^= block;
    byte_output = previous_block.MatrixToBytes();
    output_file.write(reinterpret_cast<char*>(byte_output.data()), 16);
}


if(is_decrypt)cout<<"Decryption complete"<<endl;      //Закрытие файлов, выход.
else cout<<"Encryption complete"<<endl;
output_file.close();
input_file.close();
keys.close();
}