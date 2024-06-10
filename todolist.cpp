#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <queue>
#include <algorithm>
#include <ctime>

using namespace std;

struct Deadline {
    string tanggal;
};

struct Todo {
    int id;
    string tugas;
    bool status;
    Deadline tenggat;
};

vector<Todo*> todos;
queue<int> idBebas;
int nextId = 1;

bool cekTanggal(const string& tanggal) {
    regex polaTanggal("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[012])/([0-9]{4}) (0[0-9]|1[0-9]|2[0-3]):[0-5][0-9]");
    if (regex_match(tanggal, polaTanggal)) {
        int tahun = stoi(tanggal.substr(6, 4));
        int bulan = stoi(tanggal.substr(3, 2));
        int hari = stoi(tanggal.substr(0, 2));
        int jam = stoi(tanggal.substr(11, 2));
        int menit = stoi(tanggal.substr(14, 2));

        if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) {
            if (hari > 30) {
                return false;
            }
        } else if (bulan == 2) {
            if ((tahun % 4 == 0 && tahun % 100 != 0) || tahun % 400 == 0) {
                if (hari > 29) {
                    return false;
                }
            } else {
                if (hari > 28) {
                    return false;
                }
            }
        }

        time_t sekarang = time(0);
        tm *ltm = localtime(&sekarang);

        if (tahun < 1900 + ltm->tm_year) {
            return false;
        } else if (tahun == 1900 + ltm->tm_year) {
            if (bulan < 1 + ltm->tm_mon) {
                return false;
            } else if (bulan == 1 + ltm->tm_mon) {
                if (hari < ltm->tm_mday || (hari == ltm->tm_mday && (jam < ltm->tm_hour || (jam == ltm->tm_hour && menit <= ltm->tm_min)))) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}


bool sortirID(Todo* a, Todo* b) {
    return a->id < b->id;
}

bool sortirTenggat(Todo* a, Todo* b) {
    return a->tenggat.tanggal < b->tenggat.tanggal;
}

void simpanData() {
    ofstream file("todos.txt");
    file << nextId << "\n";
    for (Todo* todo : todos) {
        file << todo->id << "\n";
        file << todo->tugas << "\n";
        file << todo->status << "\n";
        file << todo->tenggat.tanggal << "\n";
    }
    file.close();
}

void muatData() {
    ifstream file("todos.txt");
    string line;
    if (getline(file, line)) {
        nextId = stoi(line);
    }
    while (getline(file, line)) {
        Todo* todo = new Todo;
        todo->id = stoi(line);
        getline(file, todo->tugas);
        getline(file, line);
        todo->status = line == "1";
        getline(file, todo->tenggat.tanggal);
        todos.push_back(todo);
    }
    file.close();
}

void tambahTugas() {
    Todo* todo = new Todo;
    cout << "\n\tMasukkan tugas baru: ";
    cin.get();
    getline(cin, todo->tugas);
    do {
        cout << "\n\tMasukkan tanggal tenggat waktu HH/BB/TTTT JJ:MM : ";
        getline(cin, todo->tenggat.tanggal);
            if(!cekTanggal(todo->tenggat.tanggal)){
                cout <<"\tSalah, mohon diulangi";
            }
            
    } while (!cekTanggal(todo->tenggat.tanggal));
    if (!idBebas.empty()) {
        todo->id = idBebas.front(); 
        idBebas.pop();
    } else {
        todo->id = nextId++;
    }
    todo->status = false;
    todos.push_back(todo);
    cout << "\n\tTugas berhasil ditambahkan";
    simpanData();
}

void tampilkanTugasSudah(){
    for (Todo* todo : todos){
        if (todo->status){
            cout << "=========================================";
            cout << "\nID : " << todo->id;
            cout << "\nTugas : " << todo->tugas;
            cout << "\nTenggat Waktu : " << todo->tenggat.tanggal;
            cout << "\nStatus : Sudah Selesai";
            cout << "\n=========================================";
        }
    }
}

void tampilkanTugasBelum(){
    for (Todo* todo : todos){
        if (!todo->status){
            cout << "=========================================";
            cout << "\nID : " << todo->id;
            cout << "\nTugas : " << todo->tugas;
            cout << "\nTenggat Waktu : " << todo->tenggat.tanggal;
            cout << "\nStatus : Belum Selesai";
            cout << "\n=========================================";
        }
    }
}

void tampilkanTugas() {
    int pilih;
    cout << "\n\tTugas apa yang ingin ditampilkan?";
    cout << "\n\t1. Semua Tugas";
    cout << "\n\t2. Tugas yang sudah selesai";
    cout << "\n\t3. Tugas yang belum selesai";
    cout << "\n\tEnter pilih: ";
    cin >> pilih;
    cin.get();

    if (pilih == 1){
        int sortPilihan;
        cout << "\n\tSortir berdasarkan apa?";
        cout << "\n\t1. ID";
        cout << "\n\t2. Tenggat Waktu";
        cout << "\n\tMasukkan pilihan: ";
        cin >> sortPilihan;
        cin.get();
        if (sortPilihan == 1) {
            sort(todos.begin(), todos.end(), sortirID);
        } else if (sortPilihan == 2) {
            sort(todos.begin(), todos.end(), sortirTenggat);
        } else {
            cout << "\n\tPilihan Invalid";
            return;
        }
        for (Todo* todo : todos) {
            cout << "=========================================";
            cout << "\nID : " << todo->id;
            cout << "\nTugas : " << todo->tugas;
            cout << "\nTenggat Waktu : " << todo->tenggat.tanggal;
            cout << "\nStatus : " << (todo->status ? "Sudah selesai" : "Belum selesai");
            cout << "\n=========================================\n";
        }
    }else if (pilih == 2) {
        tampilkanTugasSudah();
    } else if (pilih == 3) {
        tampilkanTugasBelum();
    } else {
        cout << "\n\tPilihan Invalid";
    }
}

void centangTugas() {
    int id;
    cout << "\n\tMasukkan ID tugas untuk dicentang: ";
    cin >> id;
    for (Todo* todo : todos) {
        if (todo->id == id) {
            todo->status = true;
            cout << "\n\tTugas sudah dicentang\n";
            simpanData();
            return;
        }
    }
    cout << "\n\tTugas tidak ditemukan";
}

void hapusTugas() {
    int id;
    cout << "\n\tMasukkan ID tugas untuk dihapus: ";
    cin >> id;
    for (int i = 0; i < todos.size(); i++) {
        if (todos[i]->id == id) {
            idBebas.push(todos[i]->id);
            delete todos[i];
            todos.erase(todos.begin() + i);
            cout << "\n\tTugas berhasil dihapus\n";
            simpanData(); 
            return;
        }
    }
    cout << "\n\tTugas tidak ditemukan";
}


void editTugas() {
    int id;
    cout << "\n\tMasukkan ID tugas untuk diedit: ";
    cin >> id;
    for (Todo* todo : todos) {
        if (todo->id == id) {
            int pilih;
            cout << "\n\tApa yang mau diedit?";
            cout << "\n\t1. Tugas";
            cout << "\n\t2. tenggat waktu";
            cout << "\n\tmasukkan pilihan: ";
            cin >> pilih;
            cin.get();
            if (pilih == 1){
            cout << "\n\tMasukkan tugas baru: ";
            cin.get();
            getline(cin, todo->tugas);
            cout << "\n\tTugas berhasil diperbaharui\n";
            }else if (pilih == 2){
                do {
                cout << "\n\tMasukkan tanggal tenggat waktu baru HH/BB/TTTT JJ:MM : ";
                getline(cin, todo->tenggat.tanggal);
                if(!cekTanggal(todo->tenggat.tanggal)){
                    cout <<"\tFormat salah, mohon input ulang tanggal";
                    }
                } while (!cekTanggal(todo->tenggat.tanggal));
                    cout << "\n\tTenggat waktu berhasil diperbaharui\n";
            }   else {
                    cout << "pilihan invalid";
            }
            simpanData();
            return;

        }
            
    }
    cout << "\n\tTugas tidak ditemukan";
}

int main() {
    muatData();
        cout << "\t**************************\n";
        cout << "\t\tTo-Do-List\n";
        cout << "\t**************************";
    while (true) {
        cout << "\n\t1. Tambah Tugas";
        cout << "\n\t2. Tampilkan Tugas";
        cout << "\n\t3. Centang Tugas";
        cout << "\n\t4. Hapus Tugas";
        cout << "\n\t5. Edit Tugas";
        cout << "\n\t6. Keluar";

        int pilih;
        cout << "\n\n\tMasukkan pilihan : ";
        cin >> pilih;
        switch (pilih) {
        case 1:
            tambahTugas();
            cout<<"\n";
            break;
        case 2:
            tampilkanTugas();
            break;
        case 3:
            centangTugas();
            break;
        case 4:
            hapusTugas();
            break;
        case 5:
            editTugas();
            break;
        case 6:
            return 0;
        }
    }
}