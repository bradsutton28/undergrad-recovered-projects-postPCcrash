// Robert Allen, William Blackwell, Bradley Sutton
// Implements recursive and dynamic programming versions of Fibonacci algorithm
//test 
// *Note: File is name PA_3_Source instead of PA-3-Source because '-' is an illegal character in a Java class name

import helperclasses.Algorithm;
import helperclasses.AlgorithmResult;
import helperclasses.TestResult;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;

public class PA_3_Source {

    // Values assigned for testing
    private static final int[] FIB_TEST_VALUES = {10, 20, 30, 35, 40, 45, 50, 55};

    // Used by dynamic programming algorithm.  Allows for 'memorization' of Fib values
    private static final Map<Integer, Long> DP_MAP = new HashMap<>();

    // CSV info
    private static final String FILENAME = "Fibonacci_Time.csv";
    private static final String[] HEADERS = {
            "n", "F(n)", "T1: Recursive Algorithm (ns)/(s)/or(m)", "T2: DP Algorithm (ns)", "(2^n)/n", "T1/T2"};

    // Run Program
    public static void main(String[] args) {
        //initialize DP_MAP
        initDPArray();

        // Start interactive console with user
        fibConsole();

        // Ask user if they would like to run the tests in the given PA.  Warn the user that some values take
        // a very long time to process.
        System.out.println("\nPerform new calculations for the assigned PA values and save to file?\nAssigned Values: " +
                Arrays.toString(FIB_TEST_VALUES) + "\nCAUTION! Takes LONG Time [y/n]  ");

        // Process user choice
        String userChoice = new Scanner(System.in).next();
        if (userChoice.equalsIgnoreCase("Y")) { // yes save
            // Run Tests
            ArrayList<TestResult> results = runTests();
            try {
                writeToCSV(results);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        // Exit
        else { // no
            System.out.println("Exiting");
        }
    }

    // Measure execution time in (ns)
    private static AlgorithmResult calcTime(Algorithm fibAlg, int n) {
        long start = System.nanoTime();
        long result = fibAlg.algorithm(n);
        long stop = System.nanoTime();
        return new AlgorithmResult(result, stop - start);
    }

    // Run tests using the recursive alg and the dp alg
    private static ArrayList<TestResult> runTests() {
        ArrayList<TestResult> results = new ArrayList<>();
        for (int i : FIB_TEST_VALUES) {
            AlgorithmResult recursiveTest = calcTime(PA_3_Source::fibRecursive, i);
            System.out.println("Recursive:");
            displayResult(i, recursiveTest);

            AlgorithmResult dynamicTest = calcTime(PA_3_Source::fibDynamic, i);
            results.add(new TestResult(dynamicTest.getFibNum(), recursiveTest.getTime(), dynamicTest.getTime(), i));
            System.out.println("Dynamic Programming:");
            displayResult(i, dynamicTest);
        }
        return results;
    }

    // Calculates Fibonacci Number using Recursion
    private static long fibRecursive(int n) {
        return n <= 1 ? 1 : fibRecursive(n - 1) + fibRecursive(n - 2);
    }

    // Calculates Fibonacci Number using Dynamic Programming
    // Results of calculations saved to the DP_MAP dictionary
    private static long fibDynamic(int n) {
        // If the key already exists in DP_MAP, the algorithm has already committed it to memory, ro no need to
        // recalculate it.  Otherwise, find F(n)
        if (!DP_MAP.containsKey(n)) for (int i = 2; i <= n; i++) {
            long val = DP_MAP.get(i - 1) + DP_MAP.get(i - 2);
            DP_MAP.put(i, val);
        }
        return DP_MAP.get(n);
    }

    // Output results to csv file
    private static void writeToCSV(ArrayList<TestResult> results) throws IOException {
        PrintWriter writer = new PrintWriter(FILENAME, "UTF-8");
        writer.println(String.join(",", HEADERS));
        for (TestResult result : results) {
            writer.println(result.toString());
        }
        writer.close();
    }

    // Asks the user to select an algorithm to use to fin Fibonacci numbers.  Also displays execution time.
    private static void fibConsole() {
        Scanner scan = new Scanner(System.in);
        while (true) {
            System.out.println("Please enter (R) if you'd like to use the Recursive Fib Algorithm \n" +
                    "or enter (D) if you'd like to use the Dynamic Fib Algorithm \n" +
                    "or enter (Q) to quit.");

            // Process user input
            String algType = scan.next();

            // Quit interactive console
            if (algType.equalsIgnoreCase("Q")) {
                System.out.println("Thank you for using my program!");
                return;
            }

            // Find F(n) used to selected algorithm
            System.out.println("Please enter a positive integer value you'd like the fibonacci number to: ");
            int num = scan.nextInt();

            // Recursion
            if (algType.equalsIgnoreCase("R")) {
                displayResult(num, calcTime(PA_3_Source::fibRecursive, num));
            }
            // Dynamic
            else if (algType.equalsIgnoreCase("D")) {
                displayResult(num, calcTime(PA_3_Source::fibDynamic, num));
            }
        }
    }

    // Helper function for interactive console
    // Calculates answer from user and shows her/him the result
    private static void displayResult(int num, AlgorithmResult answer) {
        System.out.println("The value for " + num + " is: " + answer.getFibNum());
        double seconds = (double) answer.getTime() / 1_000_000_000.0;
        System.out.println("The time taken to complete the recursive algorithm for valued "
                + num + " was: " + seconds + " seconds.\n\n");
    }

    // Initialize DP_MAP
    private static void initDPArray() {
        DP_MAP.put(0, (long) 1);
        DP_MAP.put(1, (long) 1);
    }
}
