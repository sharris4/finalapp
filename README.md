# My Final App
## Sam Harris

## Why?
As many people probably agree, I love doing to weekly Sudoku puzzle in the Sunday paper. I don't, however, enjoy having to wait an entire week to check my answers. This is what made me want to put together a Sudoku Solver application.

## How?
For me, he hardest part of this project was deciding how to start. I wasn't sure exactly how I wanted to put my application together because I was afraid I'd get to a certain point, realize it was all wrong, and then start over. This is how I ended up spending more time planning than I did actually coding.
    
###   Architecture
      I realized that I was going to need to anazlyze each cell individually based on it's location in the puzzle. From that, I decided to sort the puzzle in 3 different ways knowing it could be split into 9 rows, 9 columns, and 9 blocks. Next I created a class that represented each cell and stored important information such as it's value, index, and the row, column, and block collection it belonged to. 
###   Method      
      Once I had all of this, it was fairly easy to evaluate the possible values for each cell based on missing values in it's    corresponding collections. From there I searched for cells that only had a single possible value and inserted them into the output. I repeated this until it wasn't able to find any more cells with a single possible value. Next I selected one of the few empty cell left, input it's first possible value, and attempted to solve the puzzle. If it was unsuccessful, the next possible value would be used and this would almost certainly solve the puzzle.

## What's left?
I was really hoping to improve on the way the user inputs their puzzle. Unfortunitely, it proved to be very difficult so I decided to leave the app as is.

## Where?
You can run the .exe from my app's github page
      

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/sharris4/finalapp/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
