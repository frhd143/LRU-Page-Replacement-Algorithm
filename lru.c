
/*
    Written by Farhad Asadi
    Least Recently Used Page Replacement Algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (int argc, char *argv[]) {
    int page_size = atoi(argv[2]);
    int nu_phys_pages = atoi(argv[1]);
    char *filename = argv[3];
    
    FILE *file = fopen(filename, "r");
    char line[12];
    int page_numbers[100000]; // array of page numbers
    int a = 0; // page number
    int i = 0; // keeps track of where in our array should we put our page number(a).
    
    // Open the file and save every line into our array(page_numbers)
    while(fgets(line, 13, file)) {
        a = atoi(line)/(page_size);
        page_numbers[i] = a;
        i++;        
    }
    
    fclose(file);
    
    int frames = nu_phys_pages; 
    int pages = sizeof(page_numbers)/sizeof(page_numbers[0]);
    
    // Create a table 
    int temp[frames];
    for (int i = 0; i < frames; i++) {
        // pages cannot have negative number so it is ok to till the table with -1s.
        temp[i] = -1;
    }

    int miss = 0; // number of page faults
    int index = 0; // keeps track of where we should put the new element in our array(temp)

    // Go through every page
    for (int i = 0; i < pages; i++) {
        // least_recent is the index of the least recent element used in our array(temp)
        int least_recent = 0;
        for (int j = 0; j < frames; j++) {
            if (page_numbers[i] == temp[j]) {
                /*
                least_recent can also be used for swapping our recently used element with our currently using element.
                For example, if there is already a page in our table, then least_recent can be used to update our table by making 
                that page the newest in our table. 

                               *
                pages = [3, 2, 1, 2, 4, 1, 6] and frame = 3
                Table:
                    0   [ 3 ]
                    1   [ 2 ]
                    2   [ 1 ]

                                  *
                pages = [3, 2, 1, 2, 4, 1, 6]
                Now, we get a hit (2) and we should update our table. 
                We can use least_recent to store the index of 2. 
                In line 77 (if-statement), we can see how we update our table. 
                Table:
                    0   [ 3 ]
                    1   [ 1 ]
                    2   [ 2 ]
                
                If we do not get a hit, then the least_recent is always 0 which means when updating our table we remove the first row and
                update our table. 
                */
                least_recent = j;
                // We substract one from miss because even when we get a hit the program increaments miss by 1
                // to avoid this we simply subtract one so when the program increaments one nothing will change
                miss--;
            }
        }
        miss++;

        // Remove the element in index least_recent and update the table.
        if (least_recent != 0) {
            for (least_recent; least_recent < (frames-1); least_recent++) {
                temp[least_recent] = temp[least_recent+1];
            }
            // When we updated our table, the last row becomes available. 
            index = frames - 1;
        }

        // Add the new element in the table
        if (index < frames) {
            temp[index] = page_numbers[i];
            index++;
        } else if (index == frames) {
            // When the table is full then remove the first row (which is the least recent element used in our table)
            // and update the table. 
            for (int y = 0; y < (frames-1); y++) {
                temp[y] = temp[y+1];
            }
            // Add the new element in the last row 
            temp[index-1] = page_numbers[i];
        } 
    }

    printf("Number of physical pages = %d, page size = %d\n", nu_phys_pages, page_size);
    printf("Reading memory trace from %s...  Read %d memory references\n", filename, pages);
    printf("Results:  %d page faults\n", miss);
    //printf("------------------------------------------------------------------------\n");
    //printf("%d\n", miss);
    return 0;
}



