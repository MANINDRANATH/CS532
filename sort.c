#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 1000
#define MAX_LINE_LENGTH 1024

// Structure to hold listing information
typedef struct {
    int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
    char host_name[100], neighbourhood_group[100], neighbourhood[100], room_type[100];
    float latitude, longitude, price;
} listing;

/**
 * Parses a single line of CSV input into a listing structure.
 * 
 * @param line A string containing a single line of CSV file.
 * @return listing A filled listing structure.
 */
listing parse_listing(char* line) {
    listing l;
    char* token;

    // Parse each token from the line and convert to appropriate data type
    token = strtok(line, ",");
    l.id = atoi(token);
    token = strtok(NULL, ",");
    l.host_id = atoi(token);
    token = strtok(NULL, ",");
    strcpy(l.host_name, token);
    token = strtok(NULL, ",");
    strcpy(l.neighbourhood_group, token);
    token = strtok(NULL, ",");
    strcpy(l.neighbourhood, token);
    token = strtok(NULL, ",");
    l.latitude = atof(token);
    token = strtok(NULL, ",");
    l.longitude = atof(token);
    token = strtok(NULL, ",");
    strcpy(l.room_type, token);
    token = strtok(NULL, ",");
    l.price = atof(token);
    token = strtok(NULL, ",");
    l.minimum_nights = atoi(token);
    token = strtok(NULL, ",");
    l.number_of_reviews = atoi(token);
    token = strtok(NULL, ",");
    l.calculated_host_listings_count = atoi(token);
    token = strtok(NULL, ",");
    l.availability_365 = atoi(token);

    return l;
}

/**
 * Comparison function for sorting listings by host_name.
 * 
 * @param a A pointer to the first element for comparison.
 * @param b A pointer to the second element for comparison.
 * @return int Negative if a < b, zero if a == b, positive if a > b.
 */
int compare_by_host_name(const void *a, const void *b) {
    const listing *ia = (const listing *)a;
    const listing *ib = (const listing *)b;
    return strcmp(ia->host_name, ib->host_name);
}

/**
 * Comparison function for sorting listings by price.
 * 
 * @param a A pointer to the first element for comparison.
 * @param b A pointer to the second element for comparison.
 * @return int Negative if a < b, zero if a == b, positive if a > b.
 */
int compare_by_price(const void *a, const void *b) {
    const listing *ia = (const listing *)a;
    const listing *ib = (const listing *)b;
    if (ia->price > ib->price) return 1;
    if (ia->price < ib->price) return -1;
    return 0;
}

/**
 * Writes sorted listing data to a file including a header.
 * 
 * @param filename The name of the file to write to.
 * @param listings Array of listing structures.
 * @param count The number of elements in the listings array.
 */
void write_sorted_data(const char* filename, listing* listings, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // Write header
    fprintf(file, "id,host_id,host_name,neighbourhood_group,neighbourhood,latitude,longitude,room_type,price,minimum_nights,number_of_reviews,calculated_host_listings_count,availability_365\n");

    // Write data
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%d,%s,%s,%s,%f,%f,%s,%f,%d,%d,%d,%d\n",
                listings[i].id, listings[i].host_id, listings[i].host_name,
                listings[i].neighbourhood_group, listings[i].neighbourhood,
                listings[i].latitude, listings[i].longitude, listings[i].room_type,
                listings[i].price, listings[i].minimum_nights, listings[i].number_of_reviews,
                listings[i].calculated_host_listings_count, listings[i].availability_365);
    }
    fclose(file);
}

/**
 * Main function to execute the listing management.
 * 
 * @return int Exit status.
 */
int main() {
    char line[MAX_LINE_LENGTH];
    listing listings[MAX_RECORDS];
    int count = 0;

    FILE *fptr = fopen("listings.csv", "r");
    if (!fptr) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read file and parse data
    while (fgets(line, sizeof(line), fptr)) {
        if (count < MAX_RECORDS) {
            listings[count++] = parse_listing(line);
        }
    }
    fclose(fptr);

    // Sort and write data sorted by host name
    qsort(listings, count, sizeof(listing), compare_by_host_name);
    write_sorted_data("sorted_by_host_name.csv", listings, count);

    // Sort and write data sorted by price
    qsort(listings, count, sizeof(listing), compare_by_price);
    write_sorted_data("sorted_by_price.csv", listings, count);

    return EXIT_SUCCESS;
}
