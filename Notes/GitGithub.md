---
title: Untitled

---

#Git / GitHub 
# Git Basics

- Check Git version:
    
    git --version
    
- Configure Git:
    
    git config --global [user.name](http://user.name/) "enter username"
    git config --global user.email "enter email"
    git config --global --edit
    
- Initialize a Repository:
    
    git init
    
    - Initializes the repository and creates a `.git` directory.
    - Command to check the hidden `.git` directory:
        
        ls -a
        
- Check Status:
    
    git status
    
- Add Files to Staging:
    
    git add <file>
    
- Add all files to staging:
    
    git add .
    
![Untitled (5)](https://hackmd.io/_uploads/rJCU9obtA.png)


- Commit Changes:
    
    git commit -m "initial commit"
    
- View Commit Log:
    
    git log
    

## Working with commits

- Checkout a Specific Commit:
    
    git checkout <commit hash>
    
- Checkout the Master Branch:
    
    git checkout master
    

## Branching and Merging

- Create and Switch to a New Branch:
    
    git branch dev
    git checkout dev
    
- Create and switch in one command:
    
    git checkout -b <branchname>
    
- Merge a Branch:
    
    git merge <branchname>
    
![Untitled (4)](https://hackmd.io/_uploads/rk5w5jWKA.png)


## .gitignore file

- Create a `.getignore file:
- Contains sensitive information or files you don't want to commit to GitHub.

## Remote Repositories

- Add a Remote Repository:
    
    git remote add origin https://github.com/<your-repo>.git
    
- Verify Remote URL:
    
    git remote -v
    
- Clone a Repository:
    
    git clone <repo url>
    

## File Operations

- Edit a file using Vim:
    
    vim <file>
    
- View File Contents(differences):
    
    cat <file>
    ![Untitled (3)](https://hackmd.io/_uploads/HJZQciZt0.png)
