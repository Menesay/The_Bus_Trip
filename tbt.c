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
void create_receipt(int trip_ID, char *passengers_full_name, int citizen_ID);

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
		
		printf("Enter option number: ");

		// space before &c: ignore whitespace or newline
		scanf(" %c", &choice);
		
		printf("\n");
		
		// Redirecting to the functions
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
typedef struct {
    int  trip_ID;
    char departure_point[50];
    char arrival_point[50];
    char trip_date[20];
    char trip_time[20];
    char drivers_full_name[50];
    char licence_plate[20];
    int  number_of_seat;
    int  number_of_sold_seat;
} Trip;

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// create_trip()
void create_trip(){
	
	// We are gonna use "trip" struct, with "Trip" defined struct.
	Trip trip;
	
	// Create new trip
	printf("======================\n");
	printf("== Create new trip\n");

	printf("Trip ID: ");
	// trip_ID_tmp will passed to check_ID for avoiding unnecessary struct.
	int trip_ID_tmp;
	scanf("%d", &trip_ID_tmp);
	
	
	// if check_ID == 1 then ID exist and exit.
	if (check_ID(trip_ID_tmp) == 1) { 
		printf("[ERR] ID exist."); 		
		return; 
	}
	
	
	// if check_ID == 0 add to trip struct.
	trip.trip_ID = trip_ID_tmp;


	// We must open the file after check_ID process
	// because if FILE remain in opened status, it will be resource leak.
	
	// FILE struct
	// fp represents the opened file
	// "ab" : append binary
	FILE *fp = fopen(FILE_NAME, "ab");
	
	// if fopen success then return FILE struct.
	// if fopen fail then return NULL and close the file and exit.
	if (fp == NULL) { printf("[ERR] trips.tbt couldn't opened.\n"); fclose(fp); return; }
	

	// %[^\n]s can read spaced string
    printf("Departure Point: "); scanf(" %49[^\n]s", trip.departure_point); 
    printf("Arrival Point: "); scanf(" %49[^\n]s", trip.arrival_point);
	printf("Trip Date (DD.MM.YYYY): "); scanf("%19s", trip.trip_date);
    printf("Trip Time (HH.MM): "); scanf("%19s", trip.trip_time);
	printf("Driver's Full Name: "); scanf(" %49[^\n]s", trip.drivers_full_name);
    printf("License Plate: "); scanf(" %19[^\n]s", trip.licence_plate);
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
	
	// - : write to the left
	// 8 : allocate 8 chars space
	// s : string literal
    printf("%-8s %-18s %-15s %-12s %-12s %-18s %-15s %-12s %-8s\n", 
		"Trip ID", "Departure Point", 
		"Arrival Point", "Trip Date", 
		"Trip Time", "Driver Full Name",
		"Licence Plate", "Total Seats",
		"Sold Tickets");
    
    // it retrives trip struct from trips.tbt.
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		printf("%-8d %-18s %-15s %-12s %-12s %-18s %-15s %-12d %-8d\n", 
		        trip.trip_ID, 
		        trip.departure_point, 
		        trip.arrival_point, 
		        trip.trip_date, 
		        trip.trip_time,
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

	if (fp == NULL) { printf("[INF] There is no trip.\n"); fclose(fp); return; }
	
	// Query trip
	printf("======================\n");
	printf("== Query trip\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	
    printf("%-8s %-18s %-15s %-12s %-12s %-18s %-15s %-12s %-8s\n", 
		"Trip ID", "Departure Point", 
		"Arrival Point", "Trip Date", 
		"Trip Time", "Driver Full Name",
		"Licence Plate", "Total Seats",
		"Sold Tickets");

	while (fread(&trip, sizeof(Trip), 1, fp)){
		
		// if input ID found then list.
		if (trip.trip_ID == input_ID){
			printf("%-8d %-18s %-15s %-12s %-12s %-18s %-15s %-12d %-8d\n", 
		        trip.trip_ID, 
		        trip.departure_point, 
		        trip.arrival_point, 
		        trip.trip_date,
				trip.trip_time, 
		        trip.drivers_full_name,
		        trip.licence_plate,
		        trip.number_of_seat, 
		        trip.number_of_sold_seat);	
			
			ID_found = 1;
			break;
		}
		
	}
	
	// if ID not exist then print.
	if (!ID_found){ printf("[ERR] ID not exist.\n");}
	
	fclose(fp);


	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// update_trip()
void update_trip(){
	
	int input_ID;
	int is_found = 0;
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	if (fp == NULL) { printf("[INF] There is no trip.\n"); fclose(fp); return; }
	
	// 1) were gonna write updated file to tmp.tbt
	// 2) were gonna make FILE_NAME = tmp.tbt
	FILE *fp_tmp = fopen("tmp.tbt", "wb");
	if (fp == NULL) { printf("[ERR] tmp.tbt couldn't opened.\n"); fclose(fp_tmp); return; }
	
	// Update trip
	printf("======================\n");
	printf("== Update trip\n");
	printf("Enter ID: ");
	scanf("%d", &input_ID);
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
        if (trip.trip_ID == input_ID) {
		    printf("New Departure Point: "); scanf(" %49[^\n]s", trip.departure_point); 
		    printf("New Arrival Point: "); scanf(" %49[^\n]s", trip.arrival_point);
			printf("New Trip Date (DD.MM.YYYY): "); scanf("%19s", trip.trip_date);
			printf("New Trip Time (HH.MM): "); scanf("%19s", trip.trip_time);
		    printf("New Driver's full name: "); scanf(" %49[^\n]s", trip.drivers_full_name);
		    printf("New Plate Licence: "); scanf(" %19[^\n]s", trip.licence_plate);
		    printf("New Number of seats: "); scanf("%d", &trip.number_of_seat);

			// number_of_sold_seats shouldnt be updated. its not necessary.
        
			is_found = 1;
        }
        
        // write to tmp.tbt.
        fwrite(&trip, sizeof(Trip), 1, fp_tmp);
    }

    fclose(fp);
    fclose(fp_tmp);

    if (is_found) {

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

	// i added lots of debug message error for debugging.
	
	// it was throwing permission errors all the time.
	// until i found it caused by lack of fclose

    FILE *fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) { 
        printf("[INF] There is no trip.\n"); 
        return; 
    }

    
    FILE *fp_tmp = fopen("tmp.tbt", "wb");
    if (fp_tmp == NULL) { 
        printf("[ERR] tmp.tbt couldn't created.\n");

		return; 
    }

    printf("======================\n");
    printf("== Delete trip\n");
    printf("Enter ID: ");
    scanf("%d", &input_ID);

    while (fread(&trip, sizeof(Trip), 1, fp) == 1) {

        if (trip.trip_ID == input_ID) {

            printf("[INF] ID %d found. The trips.tbt will be deleted.\n", input_ID);
            is_found = 1;

        } else {

			// if trip_ID != input_ID then write to the temp.
            if(fwrite(&trip, sizeof(Trip), 1, fp_tmp) != 1){
                printf("[ERR] Write error to tmp.tbt.\n");

            }
        }
    }

    fclose(fp);
    fclose(fp_tmp);

	// if trips.tbt contains input_ID then remove trips.tbt
	// rename(tmp.tbt, FILE_NAME)
    if (is_found) {
        
        if (remove(FILE_NAME) == 0) {
            printf("[INF] trips.tbt removed.\n");
            
            // rename it only when if it removed
            if (rename("tmp.tbt", FILE_NAME) == 0) {
                printf("[INF] trips.tbt deleted and tmp.tbt renamed.\n");
            
			} 
			else {
				
				// perror: prints stderr.
                perror("[ERR] Rename failed.");

			}
        
		}
		else {
			
            // trips.tbt cannot removed
			perror("[ERR] Cannot remove trips.tbt: ");            
			remove("tmp.tbt");
			
        }
        
    }
	else {
		// if trips.tbt not contains input_ID then remove tmp.tbt.
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
	int citizen_ID = 0;
	
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
                
				// Full name
				printf("Passenger's Full Name: ");
				scanf(" %76[^\n]s", passengers_full_name);
    			
    			// Ciziten ID
				printf("Passenger's Citizen ID: ");
				scanf(" %d", &citizen_ID);
				            
				trip.number_of_sold_seat++;
                is_found = 1;
            
				create_receipt(trip.trip_ID, passengers_full_name, citizen_ID);	
				
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
	scanf(" %76[^\n]s", passengers_full_name);
	
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		if (trip.trip_ID == input_ID) {
			
				// if sold > 0 then decrease sold.
				if(trip.number_of_sold_seat > 0) {
					
					trip.number_of_sold_seat--;
					is_found = 1;
					
					printf("[INF] Ticket cancelled.\n");
				} else {
					printf("[ERR] There is no sold ticket.\n");
				}
			
		}
		
		fwrite(&trip, sizeof(Trip), 1 ,fp_tmp);
		
	}
	
	fclose(fp);
	fclose(fp_tmp);
	
	if (is_found) {
		
		// if cancel ticket process succcess then remove the old one and rename("tmp.tbt", FILE_NAME)
		remove(FILE_NAME);
		rename("tmp.tbt", FILE_NAME);
		
		// remove receipt_trip_ID_ passengers_full_name.txt.
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
// if FILE_NAME contains input_ID then return 1.

//
// attention to fclose: if we dont use it will be RESOURCE LEAK vuln.
//

int check_ID(int trip_ID){
	
	Trip trip;
	
	FILE *fp = fopen(FILE_NAME, "rb");
	
	while (fread(&trip, sizeof(Trip), 1, fp)) {
		if (trip.trip_ID == trip_ID) {
			
			// added fclose(fp);
            // first check_ID use the file after that the others use.
			fclose(fp);
			return 1;
		
		}
		
	}
	
	// added fclose(fp);
    // first check_ID use the file after that the others use.
	fclose(fp);
	
	return 0;
	
	
}
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// create_receipt
void create_receipt(int trip_ID, char *passengers_full_name, int citizen_ID){
	
	char receipt_file_name[50];
	
	// creating unique file name.
	// receipt_tripID_passengers_full_name formatted file name creating with sprintf.
    sprintf(receipt_file_name, "receipt_%d_%s.txt",
		trip_ID, passengers_full_name);
	    
    FILE *f = fopen(receipt_file_name, "w");
    
	if (f == NULL) { printf("[ERR] receipt file couldn't opened.\n"); return; }
	
	// write to receipt_file_name with fprintf
	fprintf(f, "======================\n");
	fprintf(f, "=====THE BUS TRIP=====\n");
    fprintf(f, "========RECEIPT=======\n");
	fprintf(f, "Trip ID: %d\n", trip_ID);
    fprintf(f, "Passenger's Full Name: %s\n", passengers_full_name);
    fprintf(f, "Passenger's Citizen ID: %d\n", citizen_ID);
    fprintf(f, "======================\n");
    fclose(f);
    
}
///////////////////////////////////////////////////////




//////////////////////////////////////////////////////
// main()
int main(){
	print_banner();
    return 0;
}
//////////////////////////////////////////////////////

