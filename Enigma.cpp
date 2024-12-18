#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <array>
#include <string>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
using namespace std;

//2.5

struct enigma {
    unsigned long long int seed = 0;  //亂數種子
    unsigned int rotors = 1;  //轉子數量
    vector<int> number;  //轉子號碼
    bool set_switch[5] = {true, false, false, false, false};  //切換([0]小寫、[1]大寫、[2]數字、[3]符號、[4]隱藏輸入)
    vector<char> ex_char;  //交換線的置換表
    vector<vector<char>> sum_table;  //所有轉子的置換表
} origin, backup, setup, back_setup; //運作, 初始運作, 設定, 原始設定

void test();  //測試除錯

int rand();  //亂數生成
void reset();  //重新設定
void rotor_set(enigma &x); //設定轉子 
void rotor_number(enigma &x); //設定轉子號碼
void switching_lines(enigma &x); //設定交換線
void table_creating();  //生成置換表
void setting(); //細項設定
void refresh(); //重設ex_record
void lang(); //語言設定
void rotating(int x); //轉子旋轉

bool exactly_equal(enigma x, enigma y);  //判斷完全相等

unsigned int rotors_max = 10;  //轉子上限
int lang_index = 0;  //語言

//"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.;:?!+-*/='" \~()[]{}`@#$%^&|<>";  //總交換列表 94
/*
小寫英文字母 0+(0~25) 26
大寫英文字母 26+(0~25) 26
數字 52+(0~9) 10
標點符號 62+(0~31) 32
運算符號 68+(0~4)
額外標點符號 73+(0~3)
括號 77+(0~5)
特殊符號 83+(0~9)
空白 93+(0)
*/
string change_char = "abcdefghijklmnopqrstuvwxyz";  //默認可置換字元
int ch_lth = 26;  //默認change_char長度
string str_char[4] = {"abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "0123456789", ",.;:?!+-*/=\'\" \\~()[]{}`@#$%^&|<>"};

string reading = "";  //輸入字串
bool ex_record[93] = {0};  //紀錄交換過的位置
char swch[2] = {1,1};  //交換用暫存

vector<char> reflection;  //反射表

int i = 0;  //通用暫存變數
bool ignore_bug = false;  //解決getline後cin會忽視輸入的問題

//此處變數皆為生成亂數用
bool flip = false;  //交換XOR/XNOR邏輯防位元循環
unsigned long long int new_bit;  //計算新位元
unsigned long long int keep_unit;  //位移後的數
unsigned short X;  //PRBS的執行位置
unsigned short X_unit = 1;  //位移量
unsigned short turn_face = 0;  //旋轉模式

int main() {  //主體
    cout<<"Welcome to Enigma II."<<endl;
    lang();
    cout<<endl;
    reset();
    while (1) {  //轉換文字
        if (!origin.set_switch[4]) {
            switch (lang_index) {  //讀取文字
                case 0:  //en
                    cout<<"Enter the string to encrypt or decrypt:";
                    break;
                case 1:  //ch
                    cout<<"請輸入要加密或解密的字串:";
                    break;
            }
        }
        if (ignore_bug) {
            cin.ignore();
            ignore_bug = false;
        }
        getline(cin,reading);
        if (reading == "quit") {  //結束
            break;
        }else if (reading == "reset") {  //重設
            reset();
            continue;
        }else if (reading == "test") {  //查看現在狀況
            test();
            continue;
        }else if (reading == "reverse") {  //恢復初始狀態
            for (i = 0 ; i < origin.rotors ; i++) {
                origin.number[i] = backup.number[i];
            }
            origin.sum_table = backup.sum_table;
            switch (lang_index) {
                case 0:  //en
                    cout<<"Reset the number of each rotor."<<endl;
                    break;
                case 1:  //ch
                    cout<<"已重製轉子位置。"<<endl;
                    break;
            }
            continue;
        }else if (reading == "unlimited") {  //增加轉子上限到最大值(意義不明)
            rotors_max = -1;
            switch (lang_index) {
                case 0:  //en
                    cout<<"The upper limit of the number of rotors has now reached "<<rotors_max<<"."<<endl;
                    break;
                case 1:  //ch
                    cout<<"已擴充轉子上限至"<<rotors_max<<"。"<<endl;
                    break;
            }
            continue;
        }else if (reading == "set") {
            setting();
            continue;
        }else if (reading == "help") {
            switch (lang_index) {
                case 0:  //en
                    cout<<"Close the file to continue.";
                    break;
                case 1:  //ch
                    cout<<"關閉檔案以繼續"<<endl; 
                    break;
            }
            system("Readme.txt");
            cin.clear();
            // fflush();
            continue;
        }
        bool up = false;
        for (i = 0 ; i < reading.length() ; i++) {  //讀取每個字元
            int j = 0;
            char word = reading[i];
            if (!origin.set_switch[1]) {
                if (word >= 'A' && word <= 'Z') {  //大寫保存
                    up = true;
                    word += 32;
                }else if (word == '_') {
                    up = true;
                    continue;
                }
            }
            int ind = change_char.find(word);  //找index，若找不到則回傳-1
            if (0 <= ind) {  //主要功能區
                int index = 0;
                ind = origin.ex_char[ind];  //交換線輸入
                for (j = 0 ; j < origin.rotors ; j++) {  //置換
                    ind = origin.sum_table[j][ind];
                    // cout<<"2:"<<change_char[ind]<<endl;/////////////////////////////////////////
                }
                ind = reflection[ind];  //反射
                // cout<<"3:"<<change_char[ind]<<endl;/////////////////////////////////////////////////////////
                for (j = origin.rotors - 1 ; j >= 0 ; j--) {  //回傳
                    ind = find(origin.sum_table[j].begin(), origin.sum_table[j].end(), ind) - origin.sum_table[j].begin();
                    // cout<<"4:"<<change_char[ind]<<endl;/////////////////////////////////////////
                }
                ind = find(origin.ex_char.begin(), origin.ex_char.end(), ind) - origin.ex_char.begin();  //交換線輸出
                // cout<<"5:"<<change_char[ind]<<endl;/////////////////////////////////////////////////////
            }
            if (!origin.set_switch[1]) {
                if (up) {  //大寫回轉
                    if (ind >= 0 && ind <= 25) {
                        ind += 26;
                        up = false;
                    }else{  //無法轉大寫
                        cout<<"_";
                        up = false;
                    }
                }
            }
            if (ind < 0) {  //無法轉換則直接輸出
                cout<<word;
            }else{  //可轉換則輸出+轉動轉子
                cout<<change_char[ind];
                origin.number[0]++;
                rotating(0);
                for (int j = 1 ; j < origin.rotors ; j++) {
                    if (origin.number[j-1] == ch_lth) {
                        origin.number[j-1] = 1;
                        origin.number[j]++;
                        rotating(j);
                    }
                }
            }
        }
        cout<<endl<<endl;
    }
    return EXIT_SUCCESS;
}

int rand() {  //亂數生成
    origin.seed = origin.seed * 2532479 + 87261;
    X = origin.seed % 63;  //PRBS的執行位置 0~62
    if (origin.seed == 0 || origin.seed == 0xFFFFFFFF) {  //防卡住的位元
        flip ^= true;
    }
    if (flip) {  //將結果保留在最後一個位元
        new_bit = (~(origin.seed >> X) ^ (origin.seed >> (X - 1))) & 1;
    }else{
        new_bit = ((origin.seed >> X) ^ (origin.seed >> (X - 1))) & 1;
    }
    if (turn_face == 0) {  //0:保右右旋(插左邊)   
        keep_unit = origin.seed & (0xFFFFFFFF >> X_unit);  //刪去左邊X_unit個位元
        origin.seed = ((origin.seed & 0xFFFFFFFF << (65 - X_unit)) >> 1) | keep_unit | (new_bit << 63);  //去除右邊、右旋、放置原本位元、將移出的0改為新增位元
    }else if (turn_face == 1) {  //1:保右左旋(插中間)
        keep_unit = origin.seed & (0xFFFFFFFF >> X_unit);  //刪去左邊X_unit個位元
        origin.seed = ((origin.seed & 0xFFFFFFFF << (64 - X_unit)) << 1) | keep_unit | (new_bit << (64 - X_unit));  //去除右邊、左旋、放置原本位元、將移出的0改為新增位元
    }else if (turn_face == 2) {  //2:保左右旋(插中間)
        keep_unit = origin.seed & (0xFFFFFFFF << X_unit);  //刪去右邊X_unit個位元
        origin.seed = ((origin.seed & 0xFFFFFFFF >> (64 - X_unit)) >> 1) | keep_unit | (new_bit << X_unit);  //去除左邊、右旋、放置原本位元、將移出的0改為新增位元
    }else if (turn_face == 3) {  //3:保左左旋(插右邊)
        keep_unit = origin.seed & (0xFFFFFFFF << X_unit);  //刪去右邊X_unit個位元
        origin.seed = ((origin.seed & 0xFFFFFFFF >> (65 - X_unit)) << 1) | keep_unit | new_bit;  //去除左邊、右旋、放置原本位元、將移出的0改為新增位元
    }
    if (origin.seed % 5 == 3) {  //機率翻轉
        flip ^= true;
        turn_face = (turn_face + 1) % 4;
    }else if (origin.seed % 5 == 1) {  //機率翻轉
        flip ^= true;
        turn_face = (turn_face + 3) % 4;
    }
    X_unit = (X_unit + X) % 64 + 1;  //更新位移量 1~64
    return (unsigned int) (origin.seed / 31) % ch_lth;
}

void reset() {  //基礎設定
    origin.seed = backup.seed;
    refresh();  //清除紀錄
    rotor_set(origin);  //轉子設定
    switching_lines(origin);  //交換線設定
    backup = origin;  //保存
    table_creating();  //置換表、反射表生成 
    return;
}

void rotor_set(enigma &x) {  //轉子總設定
    switch (lang_index) {  //轉子數量
        case 0:  //en
            cout<<"Please enter the amount of rotors:";
            break;
        case 1:  //ch
            cout<<"請輸入轉子數量:";
            break;
    }
    do {
        cin>>x.rotors;
        ignore_bug = true;
        if (cin.fail()) {
            x.rotors = 0;
            cin.clear();
            cin.ignore();
            // fflush();
        }
        if (x.rotors > rotors_max) {
            switch (lang_index) {
                case 0:  //en
                    cout<<"Why do you need so many rotors???"<<endl;
                    break;
                case 1:  //ch
                    cout<<"你要醬多轉子做麼???"<<endl;
                    break;
            }
        }
    }while (x.rotors > rotors_max);
    cout<<endl;
    cin.clear();
    // fflush();
    rotor_number(x);
    return;
}

void rotor_number(enigma &x) {
    x.number.clear();
    if (x.rotors > 0) {
        switch (lang_index) {  //每個轉子的起始號碼
            case 0:  //en
                cout<<"Please enter the start position of "<<x.rotors<<" rotors(from the first rotor to the last rotor, and the position from 1 up to "<<ch_lth<<"):";
                break;
            case 1:  //ch
                cout<<"請輸入"<<x.rotors<<"個轉子的起始位置(從第一個到最後一個，且數字從 1 到 "<<ch_lth<<" ):";
                break;
        }
        for (i = 0 ; i < x.rotors ; i++) {  //紀錄起始號碼
            unsigned int storage;  //暫存
            cin>>storage;
            ignore_bug = true;
            if (cin.fail()) {
                storage = 1;
                cin.clear();
                cin.ignore();
                // fflush();
            }
            if (storage > 0 && storage <= ch_lth) {
                x.number.push_back(storage);
            }else{
                switch (lang_index) {
                    case 0:  //en
                        cout<<"Something went wrong!"<<endl;
                        break;
                    case 1:  //ch
                        cout<<"輸入出了點問題!"<<endl;
                        break;
                }
                i--;
            }
        }
        cout<<endl;
    }
    cin.clear();
    // fflush();
    return;
} 

void switching_lines(enigma &x) {
    x.ex_char.clear();
    for (i = 0 ; i < ch_lth ; i++) {  //預設交換字元
        x.ex_char.push_back(i);
    }
    switch (lang_index) {  //交換線數量
        case 0:  //en
            cout<<"How many switching lines do you want to connect(up to "<<(ch_lth/2)<<"):";
            break;
        case 1:  //ch
            cout<<"你想接多少交換線?(最多"<<(ch_lth/2)<<"條):";
            break;
    }
    do {
        cin>>i;
        ignore_bug = true;
        if (cin.fail()) {
            i = 0;
            cin.clear();
            cin.ignore();
            // fflush();
        }
        if (i > ch_lth/2 || i < 0) {
            switch (lang_index) {
                case 0:  //en
                    cout<<"Wrong number."<<endl;
                    break;
                case 1:  //ch
                    cout<<"數字錯誤。"<<endl;
                    break;
            }
        }
    }while (i > ch_lth/2 || i < 0);
    cout<<endl;
    cin.clear();
    // fflush();
    refresh();  //清除資料
    for ( ; i > 0 ; i--) {  //交換輸入列表
        switch (lang_index) {
            case 0:  //en
                cout<<"Change letter(x y):";
                break;
            case 1:  //ch
                cout<<"交換字母(x y):";
                break;
        }
        cin>>swch[0]>>swch[1];
        ignore_bug = true;
        if (cin.fail()) {
            swch[0] = 'a';
            swch[1] = 'a';
            cin.clear();
            cin.ignore();
            // fflush();
        }
        int ind0, ind1;
        ind0 = change_char.find(swch[0]);
        ind1 = change_char.find(swch[1]);
        if (ind0 < ch_lth && ind1 < ch_lth && swch[0] != swch[1]) {
            if (ex_record[ind0] || ex_record[ind1]) {
                switch (lang_index) {
                    case 0:  //en
                        cout<<"Some characters have been exchange already!"<<endl;
                        break;
                    case 1:  //ch
                        cout<<"有些字母已經交換過了!"<<endl;
                        break;
                }
                i++;
            }else{
                ex_record[ind0] = true;
                ex_record[ind1] = true;
                x.ex_char[ind0] = ind1;
                x.ex_char[ind1] = ind0;
            }
        }else{
            switch (lang_index) {
                case 0:  //en
                    cout<<"Nice try!"<<endl;
                    break;
                case 1:  //ch
                    cout<<"認真?"<<endl;
                    break;
            }
            i++;
        }
    }
    cin.clear();
    // fflush();
    return;
}

void table_creating() {  //置換表生成(轉子接線)
    origin.seed = backup.seed;
    origin.sum_table.clear();  //清除資料
    reflection.clear();  //清除資料
    refresh();  //清除紀錄
    for (i = 0 ; i < ch_lth ; i++) {  //預設交換字元
        reflection.push_back(i);
    }
    for (i = 0 ; i < ch_lth ; i++) {  //反射表
        swch[0] = i;
        if (ex_record[i]) {
            continue;
        }
        int ind = rand();
        while (ex_record[ind] || i == ind) {
            ind = rand();
        }
        swch[1] = ind;
        // cout<<swch[0]<<" "<<swch[1]<<" "<<endl;////////////////////////////////////////////////
        ex_record[i] = true;
        ex_record[ind] = true;
        reflection[i] = swch[1];
        reflection[ind] = swch[0];
    }
    int j = 0;  //暫存變數
    for (i = 0; i < origin.rotors ; i++) {  //轉子
        origin.sum_table.push_back(vector<char>());
        refresh();
        for (j = 0 ; j < ch_lth ; j++) {
            int storage = rand();
            while (ex_record[storage]){
                storage = rand();
            }
            ex_record[storage] = true;
            origin.sum_table[i].push_back(storage);
        }
        origin.sum_table[i].push_back(origin.sum_table[i][0]);
    }
    backup.sum_table = origin.sum_table;
    return;
}

void rotating(int x) {
    for (int j = 0 ; j < ch_lth ; j++) {
        origin.sum_table[x][j] = (origin.sum_table[x][j+1] + 1) % ch_lth;
    }
    origin.sum_table[x][ch_lth] = origin.sum_table[x][0];
}

void setting() {  //設定
    unsigned int page = 1;
    const unsigned int page_max = 2;  //最大頁數
    setup = origin;
    back_setup = backup;
    switch (lang_index) {
        case 0:  //en
            cout<<endl<<"setting:"<<endl;
            break;
        case 1:  //ch
            cout<<endl<<"設定:"<<endl;
            break;
    }
    bool show_no = false;
    bool no_exist = false;
    while (true) {
        system("cls");
        cout<<"["<<page<<"/"<<page_max<<"]"<<endl;
        switch (lang_index) {
            case 0:  //en
                switch (page) {
                    case 1:
                        cout<<"[1]Language :English"<<endl
                            <<"[2]Random seed :"<<back_setup.seed<<endl
                            <<"[3]Lowercase exchange :"<<(setup.set_switch[0]?"On":"Off")<<endl
                            <<"[4]Uppercase exchange :"<<(setup.set_switch[1]?"On":"Off")<<endl
                            <<"[5]Number exchange :"<<(setup.set_switch[2]?"On":"Off")<<endl
                            <<"[6]Punctuation exchange :"<<(setup.set_switch[3]?"On":"Off")<<endl
                            <<"[7]Hide the input prompt :"<<(setup.set_switch[4]?"On":"Off")<<endl
                            <<"[8]Reset the amount of roters :"<<setup.rotors<<endl
                            <<"[9]Set the position of rotors :";
                        for (i = 0 ; i < setup.rotors ; i++) {
                            cout<<setup.number[i]<<" ";
                        }
                        cout<<endl;
                        break;
                    case 2:
                        cout<<"[1]Set the start position of rotors :";
                        for (i = 0 ; i < setup.rotors ; i++) {
                            cout<<back_setup.number[i]<<" ";
                        }
                        cout<<endl;
                        cout<<"[2]Reconnect switching line :";
                        for (i = 0 ; i < ch_lth ; i++) {
                            cout<<setup.ex_char[i];
                        }
                        cout<<endl; 
                        break;
                }
                cout<<"[0]return"<<endl;
                switch (page) {
                    case 1:
                        cout<<"\t[+]next page";
                        break;
                    case page_max:
                        cout<<"[-]prev page";
                        break;
                    default:
                        cout<<"[-]prev page\t[+]next page";
                        break;
                } 
                cout<<endl;
                break;
            case 1:  //ch
                switch (page) {
                    case 1:
                        cout<<"[1]語言 :繁體中文"<<endl
                            <<"[2]亂數種子 :"<<back_setup.seed<<endl
                            <<"[3]小寫交換切換 :"<<(setup.set_switch[0]?"開啟":"關閉")<<endl
                            <<"[4]大寫交換切換 :"<<(setup.set_switch[1]?"開啟":"關閉")<<endl
                            <<"[5]數字切換交換 :"<<(setup.set_switch[2]?"開啟":"關閉")<<endl
                            <<"[6]標點符號切換交換 :"<<(setup.set_switch[3]?"開啟":"關閉")<<endl
                            <<"[7]隱藏輸入提示 :"<<(setup.set_switch[4]?"開啟":"關閉")<<endl
                            <<"[8]重設轉子數量 :"<<setup.rotors<<endl
                            <<"[9]設定轉子位置 :";
                        for (i = 0 ; i < setup.rotors ; i++) {
                            cout<<setup.number[i]<<" ";
                        }
                        cout<<endl;
                        break;
                    case 2:
                        cout<<"[1]設定轉子初始位置 :";
                        for (i = 0 ; i < setup.rotors ; i++) {
                            cout<<back_setup.number[i]<<" ";
                        }
                        cout<<endl;
                        cout<<"[2]重接交換線 :";
                        for (i = 0 ; i < ch_lth ; i++) {
                            cout<<setup.ex_char[i];
                        }
                        cout<<endl;
                        break;
                }
                cout<<"[0]返回"<<endl;
                switch (page) {
                    case 1:
                        cout<<"\t[+]下一頁";
                        break;
                    case page_max:
                        cout<<"[-]上一頁";
                        break;
                    default:
                        cout<<"[-]上一頁\t[+]下一頁";
                        break;
                } 
                cout<<endl;
                break;
        }
        if (show_no) {
            switch (lang_index) {
                case 0:  //en
                    cout<<"No exchanging open..."<<endl;
                    break;
                case 1:  //ch
                    cout<<"沒有開啟交換..."<<endl;
                    break;
            }
        }
        if (no_exist) {
            switch (lang_index) {
                case 0:  //en
                    cout<<"Function not found."<<endl;
                    break;
                case 1:  //ch
                    cout<<"功能不存在"<<endl;
                    break;
            }
        }
        reading = getch();
        show_no = false;
        no_exist = false;
        if (reading == "0") {
            if (exactly_equal(setup, origin) && exactly_equal(back_setup, backup)) {
                system("cls");
                return;
            }
            bool type_sandwich = setup.set_switch[0] || setup.set_switch[1] || setup.set_switch[2] || setup.set_switch[3];  //?洫?`?}?????? 
            bool sec_sandwich = false;
            if (!type_sandwich) {
                show_no = true;
                continue;
            }
            type_sandwich = origin.set_switch[0] != setup.set_switch[0] || origin.set_switch[1] != setup.set_switch[1] || origin.set_switch[2] != setup.set_switch[2] || origin.set_switch[3] != setup.set_switch[3];  //?洫??s???? 
            system("cls");
            if (type_sandwich) {
                switch (lang_index) {
                    case 0:  //en
                        cout<<"Warning!!! The exchange modes have been change, all the setting will be reset!"<<endl;
                        break;
                    case 1:  //ch
                        cout<<"注意！！！ 交換模式已被更改，所有設定將會被重置！"<<endl; 
                        break;
                }
            }else if (back_setup.seed != backup.seed) {
                sec_sandwich = true;
                switch (lang_index) {
                    case 0:  //en
                        cout<<"Warning!!! The seed has been change, all the setting will be reset!"<<endl;
                        break;
                    case 1:  //ch
                        cout<<"注意！！！ 種子已被更改，所有設定將會被重置！"<<endl; 
                        break;
                }
            }else if (setup.rotors != origin.rotors) {
                sec_sandwich = true;
                switch (lang_index) {
                    case 0:  //en
                        cout<<"Warning!!! The amount of rotors has been change, all the setting will be reset!"<<endl;
                        break;
                    case 1:  //ch
                        cout<<"注意！！！ 轉子數量已被更改，所有設定將會被重置！"<<endl; 
                        break;
                }
            }
            switch (lang_index) {
                case 0:  //en
                    cout<<"Save and return?[y/n/c]"<<endl;
                    break;
                case 1:  //ch
                    cout<<"保存並返回[y/n/c]"<<endl;
                    break;
            }
            char read;
            while (true) {  //確認退出
                read = getch();
                ignore_bug = true;
                if (cin.fail()) {
                    read = 'a';
                    cin.clear();
                    cin.ignore();
                    // fflush();
                }
                tolower(read);
                if (read == 'y') {  //保存
                    system("cls");
                    back_setup.set_switch[4] = setup.set_switch[4];
                    if (type_sandwich) {
                        change_char = "";
                        if (setup.set_switch[0]) {
                            change_char += str_char[0];
                        }
                        if (setup.set_switch[1]) {
                            change_char += str_char[1];
                        }
                        if (setup.set_switch[2]) {
                            change_char += str_char[2];
                        }
                        if (setup.set_switch[3]) {
                            change_char += str_char[3];
                        }
                        ch_lth = change_char.length();
                        origin = setup;
                        backup = back_setup;
                        reset();
                    }else if (sec_sandwich) {
                        origin = setup;
                        backup = back_setup;
                        origin.seed = backup.seed;
                        table_creating();
                    }else{
                        origin = setup;
                        backup = back_setup;
                    }
                    return;
                }else if (read == 'n') {  //不保存
                    system("cls");
                    return;
                }else if (read == 'c') {  //取消
                    break;
                }
            }
        }else{
            switch (page) {
                case 1:
                    if (reading == "1") {
                        system("cls");
                        lang();
                        continue;
                    }else if (reading == "2") {
                        system("cls");
                        switch (lang_index) {  //亂數種子
                            case 0:  //en
                                cout<<"Present key:"<<back_setup.seed<<endl;
                                cout<<"Please enter your new key:";
                                break;
                            case 1:  //ch
                                cout<<"現在的鑰匙:"<<back_setup.seed<<endl;
                                cout<<"請輸入新鑰匙:";
                                break;
                        }
                        cin>>back_setup.seed;
                        ignore_bug = true;
                        if (cin.fail()) {
                            back_setup.seed = 0;
                            cin.clear();
                            cin.ignore();
                            // fflush();
                        }
                        cin.clear();
                        // fflush();
                    }else if (reading == "3") {  //小寫
                        setup.set_switch[0] ^= true;
                        back_setup.set_switch[0] ^= true;
                    }else if (reading == "4") {  //大寫
                        setup.set_switch[1] ^= true;
                        back_setup.set_switch[1] ^= true;
                    }else if (reading == "5") {  //數字
                        setup.set_switch[2] ^= true;
                        back_setup.set_switch[2] ^= true;
                    }else if (reading == "6") {  //標點符號
                        setup.set_switch[3] ^= true;
                        back_setup.set_switch[3] ^= true;
                    }else if (reading == "7") {  //提示顯示
                        setup.set_switch[4] ^= true;
                        back_setup.set_switch[4] ^= true;
                    }else if (reading == "8") {  //設定轉子數量 
                        system("cls");	
                        rotor_set(setup);
                        back_setup.rotors = setup.rotors;
                        back_setup.number.clear();
                        back_setup.number = setup.number;
                    }else if (reading == "9") {  //設定origin轉子位置 
                        system("cls");
                        rotor_number(setup);
                    }else if (reading == "+") {  //第二頁 
                        page++;
                    }else{
                        no_exist = true;
                        continue;
                    }
                    break;
                case 2:
                    if (reading == "1") {  //設定backup轉子位置
                        system("cls");
                        rotor_number(back_setup);
                    }else if (reading == "2") {  //重接交換線 
                        system("cls");
                        switching_lines(setup);
                        back_setup.ex_char = setup.ex_char;
                    }else if (reading == "-") {
                        page--;
                    }else{
                        no_exist = true;
                        continue;
                    }
                    break;
                default:
                    break;
            }
        } 
    }
}

void refresh() {  //重新設定置換紀錄 ++
    int lth = 0, k = 0;
    if (origin.set_switch[0]) {  //小寫
        for (k = 0 ; k < lth+str_char[0].length() ; k++) {
            ex_record[k] = false;
        }
        lth+=str_char[0].length();
    }
    if (origin.set_switch[1]) {  //大寫
        for (k = lth ; k < lth+str_char[1].length() ; k++) {
            ex_record[k] = false;
        }
        lth+=str_char[1].length();
    }
    if (origin.set_switch[2]) {  //數字
        for (k = lth ; k < lth+str_char[2].length() ; k++) {
            ex_record[k] = false;
        }
        lth+=str_char[2].length();
    }
    if (origin.set_switch[3]) {  //符號
        for (k = lth ; k < lth+str_char[3].length() ; k++) {
            ex_record[k] = false;
        }
        lth+=str_char[3].length();
    }
    for (k = lth ; k < 94 ; k++) {
        ex_record[k] = true;
    }
//    for (k = 0 ; k < ch_lth ; k++) {  //交換線測試
//    	cout<<change_char[k]<<"";
//    }
//    cout<<endl;
//    for (k = 0 ; k < ch_lth ; k++) {  //交換線測試
//    cout<<ex_record[k]<<"";
//    }
//    cout<<endl;
}

void lang() {  //語言設定
    do {
        switch (lang_index) {
            case 0:  //en
                cout<<"Language [1]english [2]繁體中文:";
                break;
            case 1:  //ch
                cout<<"語言 [1]english [2]繁體中文:";
                break;
            default:
                cout<<"Language [1]english [2]繁體中文:";
        }
        cin>>lang_index;
        ignore_bug = true;
        if (cin.fail()) {
            lang_index = 0;
            cin.clear();
            cin.ignore();
            // fflush();
        }
        lang_index--;
        if (lang_index < 0 || lang_index > 1) {
            cout<<"Setting fail."<<endl;
        }
    }while (lang_index < 0 || lang_index > 1);
    cin.clear();
    // fflush();
    return;
}

void test() {  //測試 ++
    int j = 0;  //暫存變數
//	for (i = 0 ; i < 100 ; i++) {  //亂數測試
//		cout<<rand()<<" ";
//	}
//	cout<<endl;
//	reset();  //使用新數值
    for (j = 0 ; j < ch_lth ; j++) {  //接受交換字元對應表
        cout<<change_char[j]<<"";
    }
    cout<<endl;
    cout<<"-----------------------------------"<<endl;
    for (i = 0 ; i < ch_lth ; i++) {  //交換線測試
        cout<<change_char[origin.ex_char[i]]<<"";
    }
    cout<<endl;
    cout<<"-----------------------------------"<<endl;
    for (i = 0 ; i < origin.rotors ; i++) {  //置換表測試
        for (j = 0 ; j < ch_lth ; j++) {
            cout<<change_char[origin.sum_table[i][j]]<<"";
        }
        cout<<endl;
    }
    cout<<"-----------------------------------"<<endl;
    for (j = 0 ; j < ch_lth ; j++) {  //反射表測試
        cout<<change_char[reflection[j]]<<"";
    }
    cout<<endl;
}

bool exactly_equal(enigma x, enigma y) {
    if (x.seed != y.seed || x.rotors != y.rotors) {
        return false;
    }
    // cout<<"pass 1"<<endl;
    int count = 0;
    for (count = 0 ; count < x.rotors ; count++) {
        if (x.number[count] != y.number[count]) {
            return false;
        }
    }
    // cout<<"pass 2"<<endl;
    for (count = 0 ; count < (end(x.set_switch) - begin(x.set_switch)) ; count++) {
        if (x.set_switch[count] != y.set_switch[count]){
            return false;
        }
    }
    // cout<<"pass 3"<<endl;
    for (count = 0 ; count < x.ex_char.size() ; count++) {
        if (x.ex_char[count] != y.ex_char[count]) {
            return false;
        }
    }
    // cout<<"pass 4"<<endl;
    return true;
}

/*  工具欄

語言
switch (lang_index) {
	case 0:  //en

		break;
	case 1:  //ch

		break;
}

set專用 頁數檢測
switch (page) {
	case 1:
		
		break;
	case 2:
		
		break;
}

*/