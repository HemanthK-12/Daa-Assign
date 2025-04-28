# CS F364 - Design and Analysis of Algorithms
## Assignment 2 : Densest Subgraph Discovery

### Website
- You can access the website here : [Daa-Assignment-2](https://hemanthk-12.github.io/Daa-Assign)

### Dataset Preparation

- Uncompress the datasets if not already done and place them in the same folder as the code files.
- **Remove the first few comment lines** before giving them as input to the code files.
- Make sure the datasets **exactly** match the given file names while editing in the main method of the given codes.
- The datasets are attached in the given links below :
    - **as-caida.txt** ([Link](https://drive.google.com/file/d/1_mpWonJiL9hq3_pp2UNW9_4xwOMWXe7B/view?usp=sharing))
    - **as20000102.txt** ([Link](https://drive.google.com/file/d/1cH80v7RBTKeY7r529kRjeo08BAa8xOda/view?usp=sharing) )
    - **CA-HepTh.txt** ([Link](https://drive.google.com/file/d/1fsvhEtWCHTIrK-stuNRVPbIYIOQQM2mx/view?usp=sharing))
    - **netscience.txt** ([Link](https://drive.google.com/file/d/1RHrPzeuPVNipyBHRvoaCgmNCP4qUo75t/view?usp=sharing))

### Execution Instruction

- Before executing the code, please go to the main method of the respective file and change it to the required dataset's path.
- To execute Exact algorithm (Algo-1) in the paper "[Efficient Algorithms for Densest Subgraph Discovery](<papers/Efficient Algorithms for Densest Subgraph Discovery.pdf>)" , run the command : 
```
g++ 1-Exact.cpp -O3 -o 1 && ./1
```
- To execute CoreExact algorithm (Algo-4) code in the paper "[Efficient Algorithms for Densest Subgraph Discovery](<papers/Efficient Algorithms for Densest Subgraph Discovery.pdf>)", run the command : 
```
g++ 2-CoreExact.cpp -O3 -o 2 && ./2
```

### Individual Contribution

| S.No | Name | ID Number | Contribution |
| :---- | :---- | :---- | :---- |
| 1 | MOHAMMAD JAMBUGHODAWALA | 2022A7PS0044H | Exact Algorithm and Website |
| 2 | BHASKARA HEMANTH KARTHIKEYA GANTI | 2022A7PS0053H | CoreExact Code Implementation and Website |
| 3 | HARSH VIKRAM JAJODIA | 2022A7PS0171H | Exact code optimization and Project Report |
| 4 | VAISHNAV DEVAGUPTAPU | 2022A7PS0085H | CoreExact code optimizationa and Readme |
| 5 | ANSHUL GOPAL | 2022A7PS0009H | CoreExact code, Readme and Website Implementation |