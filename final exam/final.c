// student_score_manager_linked_list.c

#define _CRT_SECURE_NO_WARNINGS // scanf 보안 경고를 비활성화하여 C4996 오류 방지

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char name[30];
    int korean;
    int english;
    int math;
    int total;
    float average;
    int rank;
    struct Node* p_next; 
} StudentNode;

// 전역 변수
StudentNode* p_head = NULL; 
StudentNode* p_tail = NULL; 
int student_count = 0;         
const char* data_file = "student_data.dat"; // 저장할 이진 파일 이름

// --- 함수 선언 ---
void clear_input_buffer();
int print_menu();
void add_new_node(StudentNode new_data);
void add_student_info();
void calculate_scores();
void display_scores();
void read_data_from_file();
void save_data_to_file();
void free_list();

// --- 함수 구현 ---

// 입력 버퍼를 비워주는 함수
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// 2. 메뉴 출력 및 선택을 받아 반환하는 함수
int print_menu() {
    int choice;
    printf("\n[Menu]\n");
    printf("1. .dat 파일에서 데이터 읽기\n");
    printf("2. 추가 학생 정보 입력\n");
    printf("3. .dat 파일 저장\n");
    printf("4. 성적 확인 (총점/평균/등수 계산 및 출력)\n");
    printf("5. 종료\n");
    printf("-------------------\n");
    printf("선택(1~5): ");

    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return -1;
    }
    clear_input_buffer();
    return choice;
}

// 새 노드를 생성하여 리스트 끝에 추가하는 함수 (기존 AddNumber 로직 변형)
void add_new_node(StudentNode new_data) {
    StudentNode* p_new = (StudentNode*)malloc(sizeof(StudentNode)); // 새 노드를 할당
    if (p_new == NULL) {
        printf("오류: 메모리 할당에 실패했습니다.\n");
        return;
    }

    // 데이터 복사 및 다음 포인터 초기화
    *p_new = new_data;
    p_new->p_next = NULL; // 다음 노드가 없음을 명시함 

    if (p_head == NULL) {
        // 첫 노드가 추가됨
        p_head = p_new;
        p_tail = p_new;
    }
    else {
        // 기존 리스트 끝에 연결
        p_tail->p_next = p_new;
        p_tail = p_new; // p_tail에 새 노드의 주소 값을 저장
    }
    student_count++;
}

// 4. 추가 학생 정보를 입력받는 함수 (메뉴 2)
void add_student_info() {
    StudentNode new_student = { 0 }; // 새 학생 노드를 0으로 초기화
    int kr, en, ma;

    printf("--- 추가 학생 정보 입력 ---\n");
    printf("이름: ");
    if (scanf("%29[^\n]", new_student.name) != 1) {
        clear_input_buffer();
        printf("경고: 입력 오류가 발생했습니다.\n");
        return;
    }
    clear_input_buffer();

    printf("국어 점수: ");
    if (scanf("%d", &kr) != 1) { clear_input_buffer(); printf("경고: 입력 오류.\n"); return; }
    clear_input_buffer();

    printf("영어 점수: ");
    if (scanf("%d", &en) != 1) { clear_input_buffer(); printf("경고: 입력 오류.\n"); return; }
    clear_input_buffer();

    printf("수학 점수: ");
    if (scanf("%d", &ma) != 1) { clear_input_buffer(); printf("경고: 입력 오류.\n"); return; }
    clear_input_buffer();

    new_student.korean = kr;
    new_student.english = en;
    new_student.math = ma;

    add_new_node(new_student);
    printf("성공: %s 학생 정보가 추가되었습니다. (현재 총 학생 수: %d)\n", new_student.name, student_count);
}

// 총점, 평균, 등수를 계산하는 함수
void calculate_scores() {
    if (p_head == NULL) return;

    StudentNode* p_current = p_head;

    // 1단계: 총점 및 평균 계산 (리스트 순회)
    while (p_current != NULL) {
        p_current->total = p_current->korean + p_current->english + p_current->math;
        p_current->average = (float)p_current->total / 3.0f;
        p_current->rank = 1; // 등수 초기화
        p_current = p_current->p_next;
    }

    // 2단계: 등수 계산 (이중 순회)
    p_current = p_head;
    while (p_current != NULL) {
        StudentNode* p_other = p_head;
        while (p_other != NULL) {
            if (p_current->total < p_other->total) {
                p_current->rank++;
            }
            p_other = p_other->p_next;
        }
        p_current = p_current->p_next;
    }
}

// 6. 성적 확인 결과를 출력하는 함수 (메뉴 4)
void display_scores() {
    if (p_head == NULL) {
        printf("경고: 출력할 학생 데이터가 없습니다.\n");
        return;
    }

    calculate_scores(); // 총점/평균/등수 계산

    printf("\n--- 성적 확인 결과 ---\n");
    printf("----------------------------------------------------------------\n");
    printf("| 등수 | 이름         | 국어 | 영어 | 수학 | 총점 | 평균     |\n");
    printf("----------------------------------------------------------------\n");

    StudentNode* p = p_head;
    while (p != NULL) {
        printf("| %-4d | %-12s | %-4d | %-4d | %-4d | %-4d | %-8.2f |\n",
            p->rank,
            p->name,
            p->korean,
            p->english,
            p->math,
            p->total,
            p->average);
        p = p->p_next; // 다음 노드로 이동 (기존 파일의 순회 로직)
    }
    printf("----------------------------------------------------------------\n");
}

// 기존 리스트의 메모리를 모두 해제하는 함수 (기존 파일의 해제 루틴 변형)
void free_list() {
    StudentNode* p_current = p_head;
    StudentNode* p_next_node;

    // 사용한 모든 노드를 삭제합니다. 
    while (p_current != NULL) {
        p_next_node = p_current->p_next; // 다음 노드 주소 저장
        free(p_current);                 // 기억했던 주소를 사용하여 노드를 삭제함
        p_current = p_next_node;         // 다음 노드로 이동
    }
    p_head = NULL;
    p_tail = NULL; // 반복문을 나오면 p_head 값은 NULL. p_tail 값도 NULL로 변경
    student_count = 0;
}

// 3. .dat 파일에서 학생 데이터를 읽어오는 함수 (메뉴 1)
void read_data_from_file() {
    FILE* fp = NULL;
    if ((fp = fopen(data_file, "rb")) == NULL) {
        printf("경고: 파일을 열 수 없거나 파일이 존재하지 않습니다: %s\n", data_file);
        printf("      (파일은 프로그램 실행 파일(.exe)과 같은 위치에 있어야 합니다.)\n");
        return;
    }

    free_list(); // 기존 메모리 해제 후 파일에서 새로 읽음

    StudentNode temp_student;
    int students_read = 0;

    // 파일 끝까지 StudentNode 구조체 크기 단위로 읽습니다.
    while (fread(&temp_student, sizeof(StudentNode), 1, fp) == 1) {
        temp_student.p_next = NULL;
        add_new_node(temp_student);
        students_read++;
    }

    fclose(fp);
    printf("성공: 파일에서 %d명의 학생 데이터를 읽어왔습니다. (현재 총 학생 수: %d)\n",
        students_read, student_count);
}

// 5. 현재 학생 데이터를 .dat 파일에 저장하는 함수 (메뉴 3)
void save_data_to_file() {
    if (p_head == NULL) {
        printf("경고: 저장할 학생 데이터가 없습니다.\n");
        return;
    }

    FILE* fp = NULL;
    if ((fp = fopen(data_file, "wb")) == NULL) {
        printf("오류: 파일을 저장할 수 없습니다: %s\n", data_file);
        return;
    }

    StudentNode* p_current = p_head;
    size_t items_written = 0;

    // 리스트를 순회하며 노드의 내용(포인터 제외)을 파일에 씁니다.
    while (p_current != NULL) {
        // p_next 필드를 제외한 구조체 전체를 이진 파일에 기록
        if (fwrite(p_current, sizeof(StudentNode), 1, fp) == 1) {
            items_written++;
        }
        p_current = p_current->p_next;
    }

    fclose(fp);

    if (items_written == student_count) {
        printf("성공: %d명의 학생 데이터가 파일에 저장되었습니다: %s\n", student_count, data_file);
        printf("      (파일은 프로그램 실행 파일(.exe)이 있는 위치에 저장되었습니다.)\n");
    }
    else {
        printf("오류: 파일 저장 중 문제가 발생했습니다. %d개 중 %d개만 저장됨.\n", student_count, (int)items_written);
    }
}


// 메인 함수 (프로그램 시작점)
int main(void) {
    int choice;
    // main 함수는 메뉴 기반으로 프로그램을 반복 실행하도록 구성됩니다.

    while (1) {
        choice = print_menu();

        switch (choice) {
        case 1:
            read_data_from_file();
            break;
        case 2:
            add_student_info();
            break;
        case 3:
            save_data_to_file();
            break;
        case 4:
            display_scores();
            break;
        case 5:
            printf("프로그램을 종료합니다. 연결 리스트 메모리를 해제합니다.\n");
            free_list(); // 종료 시 모든 노드의 메모리 해제
            return 0;
        default:
            if (choice != -1) {
                printf("경고: 잘못된 메뉴 선택입니다.\n");
            }
            break;
        }
    }
}