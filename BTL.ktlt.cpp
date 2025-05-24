#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Khai bao cau truc mon hoc
typedef struct MonHoc {
    char tenMon[50];
    float diem;
    struct MonHoc* next;
} MonHoc;

// Khai bao cau truc sinh vien
typedef struct SinhVien {
    char maSo[20];
    char hoTen[50];
    char lop[20];
    MonHoc* dsMonHoc;
    struct SinhVien* next;
} SinhVien;

// Tao mon hoc moi
MonHoc* taoMonHoc(char ten[], float diem) {
    MonHoc* mon = (MonHoc*)malloc(sizeof(MonHoc));
    strcpy(mon->tenMon, ten);
    mon->diem = diem;
    mon->next = NULL;
    return mon;
}

// Them mon hoc vao danh sach cua sinh vien
void themMonHoc(SinhVien* sv, MonHoc* mon) {
    if (sv->dsMonHoc == NULL) {
        sv->dsMonHoc = mon;
    } else {
        MonHoc* temp = sv->dsMonHoc;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = mon;
    }
}

// Tao sinh vien moi
SinhVien* taoSinhVien(char maSo[], char hoTen[], char lop[]) {
    SinhVien* sv = (SinhVien*)malloc(sizeof(SinhVien));
    strcpy(sv->maSo, maSo);
    strcpy(sv->hoTen, hoTen);
    strcpy(sv->lop, lop);
    sv->dsMonHoc = NULL;
    sv->next = NULL;
    return sv;
}

// Them sinh vien vao danh sach
void themSinhVien(SinhVien** head, SinhVien* sv) {
    if (*head == NULL) {
        *head = sv;
    } else {
        SinhVien* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = sv;
    }
}

// Hien thi danh sach sinh vien
void hienThiDanhSach(SinhVien* head) {
    SinhVien* temp = head;
    while (temp != NULL) {
        printf("Ma so: %s, Ho ten: %s, Lop: %s\n", temp->maSo, temp->hoTen, temp->lop);
        temp = temp->next;
    }
}

// Tim sinh vien theo ma so
SinhVien* timSinhVienTheoMa(SinhVien* head, char maSo[]) {
    SinhVien* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->maSo, maSo) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Tinh diem trung binh cua sinh vien
float tinhDiemTB(SinhVien* sv) {
    MonHoc* mon = sv->dsMonHoc;
    if (mon == NULL) return 0;
    float tong = 0;
    int dem = 0;
    while (mon != NULL) {
        tong += mon->diem;
        dem++;
        mon = mon->next;
    }
    return (dem == 0) ? 0 : tong / dem;
}

// Xep loai theo diem
const char* xepLoai(float diemTB) {
    if (diemTB >= 8.0) return "Gioi";
    if (diemTB >= 6.5) return "Kha";
    if (diemTB >= 5.0) return "Trung binh";
    return "Yeu";
}

// Thong ke dau/rot
void thongKeDauRot(SinhVien* head) {
    int dau = 0, rot = 0;
    while (head != NULL) {
        float dtb = tinhDiemTB(head);
        if (dtb >= 5.0)
            dau++;
        else
            rot++;
        head = head->next;
    }
    printf("So sinh vien dau: %d\n", dau);
    printf("So sinh vien rot: %d\n", rot);
}

// Giai phong bo nho
void giaiPhong(SinhVien* head) {
    while (head != NULL) {
        MonHoc* m = head->dsMonHoc;
        while (m != NULL) {
            MonHoc* tmpM = m;
            m = m->next;
            free(tmpM);
        }
        SinhVien* tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Luu vao file
void luuVaoFile(SinhVien* head, const char* tenFile) {
    FILE* f = fopen(tenFile, "w");
    if (!f) {
        printf("Khong mo duoc file!\n");
        return;
    }
    while (head != NULL) {
        fprintf(f, "%s;%s;%s\n", head->maSo, head->hoTen, head->lop);
        MonHoc* m = head->dsMonHoc;
        while (m != NULL) {
            fprintf(f, "%s,%.2f\n", m->tenMon, m->diem);
            m = m->next;
        }
        fprintf(f, "#\n"); // Danh dau ket thuc mon hoc cua 1 sinh vien
        head = head->next;
    }
    fclose(f);
}

// Doc tu file
SinhVien* docTuFile(const char* tenFile) {
    FILE* f = fopen(tenFile, "r");
    if (!f) {
        printf("Khong mo duoc file!\n");
        return NULL;
    }
    SinhVien* head = NULL;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        if (strchr(line, ';')) {
            char maSo[20], hoTen[50], lop[20];
            sscanf(line, "%[^;];%[^;];%[^\n]", maSo, hoTen, lop);
            SinhVien* sv = taoSinhVien(maSo, hoTen, lop);
            while (fgets(line, sizeof(line), f)) {
                if (line[0] == '#') break;
                char tenMon[50];
                float diem;
                sscanf(line, "%[^,],%f", tenMon, &diem);
                themMonHoc(sv, taoMonHoc(tenMon, diem));
            }
            themSinhVien(&head, sv);
        }
    }
    fclose(f);
    return head;
}

// Xuat bao cao ra file
void xuatBaoCao(SinhVien* head, const char* tenFile) {
    FILE* f = fopen(tenFile, "w");
    while (head != NULL) {
        fprintf(f, "Ho ten: %s - Lop: %s\n", head->hoTen, head->lop);
        MonHoc* m = head->dsMonHoc;
        while (m != NULL) {
            fprintf(f, "  %s: %.2f\n", m->tenMon, m->diem);
            m = m->next;
        }
        float dtb = tinhDiemTB(head);
        fprintf(f, "  Diem TB: %.2f - Xep loai: %s\n\n", dtb, xepLoai(dtb));
        head = head->next;
    }
    fclose(f);
}

// Ham main - menu chinh
int main() {
    SinhVien* head = NULL;
    int choice;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Them sinh vien\n");
        printf("2. Hien thi danh sach\n");
        printf("3. Tim sinh vien theo ma\n");
        printf("4. Hien thi chi tiet sinh vien\n");
        printf("5. Thong ke dau rot\n");
        printf("6. Luu vao file\n");
        printf("7. Doc tu file\n");
        printf("8. Xuat bao cao\n");
        printf("0. Thoat\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);
        getchar(); // Xoa '\n'

        if (choice == 1) {
            char maSo[20], hoTen[50], lop[20];
            printf("Nhap ma so: "); fgets(maSo, sizeof(maSo), stdin);
            printf("Nhap ho ten: "); fgets(hoTen, sizeof(hoTen), stdin);
            printf("Nhap lop: "); fgets(lop, sizeof(lop), stdin);
            maSo[strcspn(maSo, "\n")] = 0;
            hoTen[strcspn(hoTen, "\n")] = 0;
            lop[strcspn(lop, "\n")] = 0;

            SinhVien* sv = taoSinhVien(maSo, hoTen, lop);
            int soMon;
            printf("Nhap so mon hoc: ");
            scanf("%d", &soMon);
            getchar();
            for (int i = 0; i < soMon; i++) {
                char tenMon[50];
                float diem;
                printf("  Mon %d - Ten: ", i + 1);
                fgets(tenMon, sizeof(tenMon), stdin);
                tenMon[strcspn(tenMon, "\n")] = 0;
                printf("  Diem: ");
                scanf("%f", &diem);
                getchar();
                themMonHoc(sv, taoMonHoc(tenMon, diem));
            }
            themSinhVien(&head, sv);
        } else if (choice == 2) {
            hienThiDanhSach(head);
        } else if (choice == 3) {
            char ma[20];
            printf("Nhap ma so: ");
            fgets(ma, sizeof(ma), stdin);
            ma[strcspn(ma, "\n")] = 0;
            SinhVien* sv = timSinhVienTheoMa(head, ma);
            if (sv)
                printf("Tim thay: %s - %s\n", sv->hoTen, sv->lop);
            else
                printf("Khong tim thay!\n");
        } else if (choice == 4) {
            char ma[20];
            printf("Nhap ma so: ");
            fgets(ma, sizeof(ma), stdin);
            ma[strcspn(ma, "\n")] = 0;
            SinhVien* sv = timSinhVienTheoMa(head, ma);
            if (sv) {
                printf("Ho ten: %s\n", sv->hoTen);
                printf("Lop: %s\n", sv->lop);
                MonHoc* m = sv->dsMonHoc;
                if (!m) printf("Khong co mon hoc nao.\n");
                else {
                    while (m) {
                        printf("  %s: %.2f\n", m->tenMon, m->diem);
                        m = m->next;
                    }
                }
                float dtb = tinhDiemTB(sv);
                printf("Diem TB: %.2f (%s)\n", dtb, xepLoai(dtb));
            } else printf("Khong tim thay!\n");
        } else if (choice == 5) {
            thongKeDauRot(head);
        } else if (choice == 6) {
            luuVaoFile(head, "sinhvien.txt");
        } else if (choice == 7) {
            giaiPhong(head);
            head = docTuFile("sinhvien.txt");
            printf("Da doc tu file.\n");
        } else if (choice == 8) {
            xuatBaoCao(head, "baocao.txt");
            printf("Da xuat bao cao vao file.\n");
        }
    } while (choice != 0);

    giaiPhong(head);
    return 0;
}

