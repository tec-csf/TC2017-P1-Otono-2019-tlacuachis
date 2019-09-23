
import java.io.File;
import java.io.IOException;
import java.util.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author swapnil
 */
public class main {
    public static void main(String[] args) throws IOException {
        TwoThreeTree tree = new TwoThreeTree();

        Scanner sc = new Scanner(System.in);
        boolean menuWhile = true;
        int menu =0, value=0;
        //Array Declaration
        int arrsize = 100000;
        int arr[]=new int[arrsize];
        tree.scramble_array(arr, arrsize);

        //Insert array into the tree
        for(int i = 0; i<arrsize-10;i++){
            boolean success = tree.insert(arr[i]);
                    if (success) {
                        // If the value was inserted
                        //System.out.printf("Key %s inserted\n", arr[i]);
                    } else {
                        // If insertion fails
                        //System.out.printf("Key %s already exists\n", arr[i]);
                    } // end of if else
        }

        //MENU
        while(menuWhile){
            System.out.print("--------2-3 Tree Main Menu-------- \n 1. Search Element \n 2. Print Postorder \n 3. Print Inorder \n 4. Insert Element \n 5. Eliminar Elemento \n 6. Obtain Node Level \n 7. Obtain Node Depth \n 8. Obtain Node Height \n 0. Leave Menu \n");
            menu = sc.nextInt();
        
            if(menu ==0){
                menuWhile=false;
            }
        
            else if(menu == 1){
                System.out.print("Buscar en el arbol por el elemento: ");
                value = sc.nextInt();
                long startTime = System.nanoTime();
                boolean success = tree.search(value);
                long endTime = System.nanoTime();
                System.out.println("That took " + (endTime - startTime) + " nanoseconds");
                    if (success) {
                        // If the search finds the key
                        System.out.printf("Key %s found\n", value);
                    } else {
                        // If the key wasn't found
                        System.out.printf("Key %s doesn't exist\n", value);
                    } // end of if else

                }//End search if

            else if(menu == 4){
                // Get value to be inserted
                System.out.print("Insert the value to get inserted: ");
                value = sc.nextInt();
                // Try to insert the value and return result                    
                boolean success = tree.insert(value);
                if (success) {
                    // If the value was inserted
                    System.out.printf("Key %s inserted\n", value);
                } else {
                    // If insertion fails
                    System.out.printf("Key %s already exists\n", value);
                } // end of if else
            }//End Insert if

            else if(menu == 5){
            // Get value to be deleted
            System.out.print("Insert the value to get deleted: ");
            value = sc.nextInt();
            // Try to insert the value and return result                    
            boolean success = tree.remove(value);
            if (success) {
                // If the value was inserted
                System.out.printf("Key %s deleted\n", value);
            } else {
                // If deletion fails
                System.out.printf("Key %s doesn't exist\n", value);
            } // end of if else
        }//End Delete if

            else{
                System.out.print("Wrong input, try again.");
            }
        }
    } // end of main
}
