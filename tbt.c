/////////////////////////////////////////////////////
// 
// The Bus Trip
// 
// Licenced: GPL
// Contributors: Menesay
// Date: Nov 2025
/////////////////////////////////////////////////////



/////////////////////////////////////////////////////
// libraries

// I/O
#include <stdio.h>
// memory alloc
#include <stdlib.h>
// string
#include <string.h>

// [DBG]
// sleep fonksiyonu için
// #include <windows.h>

/////////////////////////////////////////////////////


/////////////////////////////////////////////////////
// Function Definitions

// trip
void create_trip();
void list_trip();
void query_trip();
void update_trip();
void delete_trip();

// ticket
void sell_ticket();
void cancel_ticket();

// create receipt
// it will run in sell_ticket
void create_receipt(int trip_ID, char *passengers_full_name);

// check ID
int check_ID(int trip_ID);

// print banner
void print_banner();
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// print_banner()
void print_banner(){
	
	char choice;
	
	while (7){
	
		printf("\n\n\n");
		printf("======================\n");
		printf("=====THE BUS TRIP=====\n");
		printf("======================\n");
		printf("1 Create trip\n");
		printf("2 List trips\n");
		printf("3 Query trip\n");
		printf("4 Update trip\n");
		printf("5 Delete trip\n");
		printf("6 Sell ticket\n");
		printf("7 Cancel ticket\n");
		printf("0 Exit\n");
		printf("======================\n");
		
		printf("Enter number: ");

		// &c'den önce boşluk: whitespace veya newlinei ignore et
		scanf(" %c", &choice);
		
		// güzel görünsün diye
		printf("\n");
		
		// Redirect to the function
		switch(choice){
			
			case '1': create_trip();   break;
			case '2': list_trip();     break;
			case '3': query_trip();    break;
			case '4': update_trip();   break;
			case '5': delete_trip();   break;
			case '6': sell_ticket();   break;
			case '7': cancel_ticket(); break;
			
			case '0': printf("[INF] Exiting\n"); exit(0);
			
			default:  printf("[ERR] Invalid choice.\n");
			
		}
	}
}
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// filename
// .tbt : the bus trip
const char *FILE_NAME = "trips.tbt";
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// trip struct
// bus's trip ID, departure point, arrival point, 
// trip date, departure time, bus license plate,
// driver's full name, number of seats and any other information.
// verisi tutulacak
typedef struct {
    int  trip_ID;
    char departure_point[50];
    char arrival_point[50];
    char trip_date[20];
    char drivers_full_name[50];
    char licence_plate[20];
    int  number_of_seat;
    int  number_of_sold_seat;
} Trip;

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// create_trip()
void create_trip(){
	
	// "Trip" tanımlı struct sayesinde "trip" adlı struct objesini kullanıcaz.
	Trip trip;
	
	// FILE structı
	// fp represents the opened file
	// "ab" : append binary
	FILE *fp = fopen(FILE_NAME, "ab");
	
	// fopen başarılı olursa FILE struct return eder
	// fopen başarısız olursa NULL return eder.
	if (fp == NULL) { printf("[ERR] trips.tbt couldn't opened.\n"); return; }
	
	// Create new trip
	printf("======================\n");
	printf("== Create new trip\n");

	printf("Trip ID: ");
	
	// check_ID'ye trip_ID_tmp verilecek ki gereksiz struct oluşturulmasın.
	int trip_ID_tmp;
	scanf("%d", &trip_ID_tmp);
	
	// [DBG] RESOURCE LEAK !!! comment out ediyorum.
	/*
	if (check_ID(input_ID) == 0) { 
		
		printf("[ERR] ID not exist."); 		
		return; 
	
	}
	*/

	// check_ID == 0 ise structa ekle.
	trip.trip_ID = trip_ID_tmp;

	// %[^\n]s can read spaced string
    printf("Departure Point: "); scanf(" %[^\n]s", trip.departure_point); 
    printf("Arrival Point: "); scanf(" %[^\n]s", trip.arrival_point);
    
	printf("Trip Date (DD.MM.YYYY): "); scanf("%s", trip.trip_date);
    printf("Driver's Full Name: "); scanf(" %[^\n]s", trip.drivers_full_name);
    printf("License Plate: "); scanf(" %[^\n]s", trip.licence_plate);
    printf("Number of seats: "); scanf("%d", &trip.number_of_seat);
    
    // Default 0.
	trip.number_of_sold_seat = 0;

    fwrite(&trip, sizeof(Trip), 1, fp);
    fclose(fp);
    printf("[INF] Trip added.\n");
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// list_trip()
void list_trip(){
	
	Trip trip;
	
	// read binary
    FILE *fp = fopen(FILE_NAME, "rb");
    
    if (fp == NULL) { printf("[INF] There is no trip.\n"); return; }

	// list trips
	printf("======================\n");
	printf("== List trips\n");
	
	// - : sola yaz
	// 8 : 8 karakterlik alan ver
	// s : string
    printf("%-8s %-18s %-18s %-12s %-18s %-15s %-12s %-8s\n", 
		"Trip ID", "Departure Point", 
		"Arrival Point", "Trip Date", 
		"Driver Full Name", "Licence Plate",
		"Total Seats", "Sold Tickets");
    
    // trip struct'ını trips.tbt'den retrieve ediyor.
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		printf("%-8d %-18s %-18s %-12s %-18s %-15s %-12d %-8d\n", 
		        trip.trip_ID, 
		        trip.departure_point, 
		        trip.arrival_point, 
		        trip.trip_date, 
		        trip.drivers_full_name, 
		        trip.licence_plate,
		        trip.number_of_seat, 
		        trip.number_of_sold_seat);
    }
    fclose(fp);
	
	printf("[INF] Trips listed.\n");
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// query_trip()
void query_trip(){

	int input_ID;
	int ID_found;
	Trip trip;
	FILE *fp = fopen(FILE_NAME, "rb");

	if (fp == NULL) { printf("[INF] There is no trip.\n"); return; }
	
	// Query trip
	printf("======================\n");
	printf("== Query trip\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	
	printf("%-8s %-18s %-18s %-12s %-18s %-15s %-12s %-8s\n", 
		"Trip ID", "Departure Point", 
		"Arrival Point", "Trip Date", 
		"Driver Full Name", "Licence Plate",
		"Total Seats", "Sold Tickets");

	while (fread(&trip, sizeof(Trip), 1, fp)){
		
		// input ID bulunursa listele
		if (trip.trip_ID == input_ID){
			printf("%-8d %-18s %-18s %-12s %-18s %-15s %-12d %-8d\n", 
		        trip.trip_ID, 
		        trip.departure_point, 
		        trip.arrival_point, 
		        trip.trip_date, 
		        trip.drivers_full_name,
		        trip.licence_plate,
		        trip.number_of_seat, 
		        trip.number_of_sold_seat);	
			
			ID_found = 1;
			break;
		}
		
	}
	
	// ID yoksa print et.
	if (!ID_found){ printf("[ERR] ID not exist.\n");}
	
	fclose(fp);


	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// update_trip()
void update_trip(){
	
	int input_ID;
	
	// 0'la başlamalı
	// yoksa ilerdeki if(is_found) şans eseri 1 gelebilir.
	int is_found = 0;
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	if (fp == NULL) { printf("[INF] There is no trip.\n"); return; }
	
	// 1) tmp.tbt'ye yeni güncellenmiş fileı yazıcaz
	// 2) tmp.tbt'yi FILE_NAME olarak güncellicez
	FILE *fp_tmp = fopen("tmp.tbt", "wb");
	if (fp == NULL) { printf("[ERR] tmp.tbt couldn't opened.\n"); return; }
	
	// Update trip
	printf("======================\n");
	printf("== Update trip\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
        if (trip.trip_ID == input_ID) {
		    printf("New Departure Point: "); scanf(" %[^\n]s", trip.departure_point); 
		    printf("New Arrival Point: "); scanf(" %[^\n]s", trip.arrival_point);
			printf("New Trip Date (DD.MM.YYYY): "); scanf("%s", trip.trip_date);
		    printf("New Driver's full name: "); scanf(" %[^\n]s", trip.drivers_full_name);
		    printf("New Plate Licence: "); scanf(" %[^\n]s", trip.licence_plate);
		    printf("New Number of seats: "); scanf("%d", &trip.number_of_seat);

			// number_of_sold_seats'e dokunmuyoruz. çünkü bu usera teslim edilmemeli
			// program otomatik kendisi halledicek
			// sell_ticket arttırır
			// cancel_ticket azaltır
          
			is_found = 1;
        }
        
        // tmp.tbt'ye yaz.
        fwrite(&trip, sizeof(Trip), 1, fp_tmp);
    }

    fclose(fp);
    fclose(fp_tmp);

    if (is_found) {
    	// OS bizim için siliyor.
        remove(FILE_NAME);
        
		rename("tmp.tbt", FILE_NAME);
        printf("[INF] Trip updated.\n");
    
	} else {
        remove("tmp.tbt");
        printf("[ERR] ID not exist.\n");
    }
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// delete_trip()
void delete_trip() {
	
    int input_ID;
    int is_found = 0;
    Trip trip;

	// [DBG]
	// bi türlü çalışmadı
	// ben de **aşırı agresif debug** ekledim tüm adımlar kontrol ediliyor.
	
	// şimdi program çalıştırılınca permission error veriyor.
	// bu administratorluk ile ilgili değil!
	// sanırım race condition hatası oluyor.
	// OS fileı kapatamadan ben remove ve rename yapıyorum ve
	// PATlıyor

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) { 
        printf("[INF] There is no trip.\n"); 
        return; 
    }

    
    FILE *fp_tmp = fopen("tmp.tbt", "wb");
    if (fp_tmp == NULL) { 
        printf("[ERR] tmp.tbt couldn't created.\n");
        
		
		// [DBG]
		// fclose(fp);
        
		
		return; 
    }

    printf("======================\n");
    printf("== Delete trip\n");
    printf("Enter ID: ");
    scanf("%d", &input_ID);

	// ID check
	
	
	// [DBG] RESOURCE LEAK !!! comment out ediyorum.
	// çalıştı :)
	/*
	// ID exits olmalı.
	if (check_ID(input_ID) == 0) { 
		
		printf("[ERR] ID not exist."); 		
		return; 
	
	}

	*/

	
	// == 1 'e gerek yok ama OLSUN!
    while (fread(&trip, sizeof(Trip), 1, fp) == 1) {

        if (trip.trip_ID == input_ID) {

            printf("[INF] ID %d found. The trips.tbt will be deleted.\n", input_ID);
            is_found = 1;

        } else {

			// ID eşleşmediyse tempe yaz
            if(fwrite(&trip, sizeof(Trip), 1, fp_tmp) != 1){
                printf("[ERR] Write error to tmp.tbt.\n");

            }
        }
    }

    // fileları kapayıtoruz ama OS sanırım yavaş kapatıyor.
    // permission error oluyor!.
    fclose(fp);
    fclose(fp_tmp);

	// [DBG]
	// 1sn bekle
	// Sleep(1000);

	// istenen ID trips.tbt'de varsa o dosya silinecek.
	// tmp.tbt, yeni trips.tbt olacak.
    if (is_found) {
        
        if (remove(FILE_NAME) == 0) {
            printf("[INF] trips.tbt removed.\n");
            
            // [DBG]
			// 1sn bekle
			// Sleep(1000);
            
            // silinirse rename yap
            if (rename("tmp.tbt", FILE_NAME) == 0) {
                printf("[INF] trips.tbt deleted and tmp.tbt renamed.\n");
            
			} 
			else {
				
				// perror: ERROR sebebi print eder.
                perror("[ERR] Rename failed.");
                
            
			}
        
		}
		else {
			
			// [DBG] burada takılıyor.
			// FILE_NAME silinemiyor.
			
            // trips.tbt silinemedi
			perror("[ERR] Cannot remove trips.tbt: ");            
			remove("tmp.tbt");
			
        }
        
    }
	else {
		// istenen ID yoksa tmp.tbt'yi sil.
        remove("tmp.tbt");
        printf("[ERR] ID %d not exist.\n", input_ID);
    }
    
    
}

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// sell_ticket
void sell_ticket(){
	
	int input_ID;
	int is_found = 0;
	char passengers_full_name[77];
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	if (fp == NULL) { printf("[INF] There is no trip.\n"); return; }
	FILE *fp_tmp = fopen("tmp.tbt", "wb");
	if (fp_tmp == NULL) { printf("[ERR] tmp.tbt couldn't opened.\n"); return; }
	
	// Sell ticket
	printf("======================\n");
	printf("== Sell ticket\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
        if (trip.trip_ID == input_ID) {
            
			if (trip.number_of_sold_seat < trip.number_of_seat) {
                
				printf("Passenger's Full Name: ");
				scanf(" %[^\n]s", passengers_full_name);
                
				trip.number_of_sold_seat++;
                is_found = 1;
            
            	// create_receipte trip_ID ve passengers_full_name'i ver
				create_receipt(trip.trip_ID, passengers_full_name);	
				
				printf("[INF] Ticket sold. Receipt created.\n");
            
			} else {
                printf("[ERR] Seats are full.\n");
            }
        
		}
        fwrite(&trip, sizeof(Trip), 1, fp_tmp);
    }

    fclose(fp);
    fclose(fp_tmp);
	
	
	if (is_found) {
        remove(FILE_NAME);
        rename("tmp.tbt", FILE_NAME);
    } else {
        remove("tmp.tbt");
        if(!is_found) printf("[ERR] ID not exist.\n");
    }
	
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// Cancel ticket
void cancel_ticket(){
	
	int input_ID;
	int is_found = 0;
	
	char passengers_full_name[77];
	char receipt_file_name[50];
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	if (fp == NULL) { printf("[INF] There is no trip.\n"); return; }
	FILE *fp_tmp = fopen("tmp.tbt", "wb");
	if (fp_tmp== NULL) { printf("[ERR] tmp.tbt couldn't opened.\n"); return; }
	
	// Cancel ticket
	printf("======================\n");
	printf("== Cancel  ticket\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	printf("Passenger's Full Name: ");
	scanf(" %[^\n]s", passengers_full_name);
	
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		if (trip.trip_ID == input_ID) {
			
				// satılan > 0 ise azalt.
				if(trip.number_of_sold_seat > 0) {
					
					trip.number_of_sold_seat--;
					is_found = 1;
					
					printf("[INF] Ticket cancelled.\n");
				} else {
					printf("[ERR] There is no sold ticket.\n");
				}
			
		}
		
		// Güncellenmiş bilgilere göre temp e yaz hepsini
		fwrite(&trip, sizeof(Trip), 1 ,fp_tmp);
		
	}
	
	fclose(fp);
	fclose(fp_tmp);
	
	if (is_found) {
		
		// eğer cancel ticket işlemi gerçekleşirse eskiyi sil ve tempi yeni dosya yap.
		remove(FILE_NAME);
		rename("tmp.tbt", FILE_NAME);
		
		// receipt_trip_ID_ passengers_full_name.txt'yi sil.
		sprintf(receipt_file_name, "receipt_%d_%s.txt",
		input_ID, passengers_full_name);
		remove(receipt_file_name);
		
	} else {
		remove("tmp.tbt");
	}
	
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// Check ID
// input_ID FILE_NAME'de varsa 1 return eder.

//
// [DBG] flcose yok! RESOURCE LEAK vuln
//
/*
int check_ID(int trip_ID){
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		if (trip.trip_ID == trip_ID) {
			return 1;
		}
		
	}
	return 0;
	
	
}
*/
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// create_receipt
void create_receipt(int trip_ID, char *passengers_full_name){
	
	char receipt_file_name[50];
	
	// unique file name oluşturuyor.
	// snprintf ile receipt_tripID_passengers_full_name formatında file name oluşturuluyor.
	// n: buffer overflow koruması file name buffer sizeı ile kısıtlanıyor.
	// [DEV] buffer overflow  önlemi!
    sprintf(receipt_file_name, "receipt_%d_%s.txt",
		trip_ID, passengers_full_name);
	    
    FILE *f = fopen(receipt_file_name, "w");
    
	if (f == NULL) { printf("[ERR] receipt file couldn't opened.\n"); return; }
	
	// fprintf ile receipt_file_name'e yaz
	fprintf(f, "======================\n");
	fprintf(f, "=====THE BUS TRIP=====\n");
    fprintf(f, "========RECEIPT=======\n");
	fprintf(f, "Trip ID: %d\n", trip_ID);
    fprintf(f, "Passenger's Full Name: %s\n", passengers_full_name);
    fprintf(f, "======================\n");
    fclose(f);
    
}
///////////////////////////////////////////////////////




//////////////////////////////////////////////////////
// main()
int main(){
	print_banner();
    printf("this is windows");
    return 0;
}
//////////////////////////////////////////////////////

