# Git & GitHub 入門課程

## 講者

何承軒 · Riley Ho

- github.com/rileychh
- instagram.com/rileychh.jpg
- [簡歷](https://nc.rileychh.dev/s/aJrmoSkMa4sEgXi)

## 什麼是 Git？為什麼要學？

你有沒有遇過這種情況：

- 在檔案夾裡存了好幾個版本的文件，檔名亂到不行，例如：`project_final_v2_real_final.docx`？
- 修改了程式碼或文件後，發現一堆地方被改壞，卻找不到上一個版本的內容？
- 跟別人一起合作一個專案，檔案來回傳遞，版本衝突到讓人崩潰？

別擔心！**Git** 就是用來解決這些問題的。

Git 是一個**版本控制系統**，能夠記錄你所有的檔案變更，保留歷史版本，還能讓多人協作更加順利。不論你是寫程式、寫論文，甚至做設計，學會 Git 都能讓你的工作更有條理。

## Git 的歷史

Git 是由著名的程式設計師 Linus Torvalds 於 2005 年所創建，他同時也是 Linux 作業系統的作者。當時，Linux
開發團隊需要一個高效且可靠的版本控制工具，但市面上現有的解決方案都不符合需求。因此，Linus 在短短幾週內設計出了 Git，並讓它成為一個自由且開源的工具。隨著時間過去，Git
已經成為全球最流行的版本控制系統，廣泛應用於程式開發、文件編寫和協作專案。

## 安裝 Git

開始之前，我們先安裝 Git。

### 下載 Git

- **Windows 使用者**：[前往官網下載](https://git-scm.com/downloads)，下載並安裝 Git。

- **Mac 使用者**：打開終端機（Terminal），輸入：

    ```bash
    git --version
    ```

    如果 Git 沒有安裝，你可以使用 Homebrew 安裝：

    ```bash
    brew install git
    ```

- **Linux 使用者**：根據你的發行版，輸入：

    ```bash
    sudo apt install git   # Ubuntu/Debian
    sudo yum install git   # CentOS/RHEL
    ```

### 確認安裝成功

安裝完成後，打開終端機（Command Line/Terminal）並輸入：

```bash
git --version
```

若出現類似 `git version 2.x.x` 的訊息，恭喜你，Git 已經安裝成功了！

## 開始實作：建立你的第一個 Git 專案

現在我們來實際操作一遍！

### 步驟一：建立一個新資料夾

打開終端機，選擇一個你想放專案的地方，輸入：

```bash
mkdir project # 建立新資料夾
cd project    # 進入資料夾
```

### 步驟二：初始化 Git

在開始之前，我們需要設定 Git 的**作者名字**和 **Email 地址**，這樣每次提交時，Git 才能記錄是誰做的變更。

輸入以下指令設定你的名字和 Email：

```bash
git config --global user.name "你的名字"
git config --global user.email "你的 Email 地址"
```

- `user.name` 是你的名稱，可以用真名或暱稱。
- `user.email` 是你的電子郵件地址。

> [!NOTE]
> 在 GitHub上操作時，你的名字是個人檔案設定的顯示名稱，電子郵件地址是[設定](https://github.com/settings/emails#primary_email_select_label)中的「Primary
> email address」。

你可以用以下指令確認設定是否成功：

```bash
git config --list
```

接著，我們來初始化一個 Git 版本庫：

```bash
git init
```

你會看到一個訊息：`Initialized empty Git repository...`，代表 Git 已經準備好幫你追蹤檔案了。

### 步驟三：新增檔案並提交

1. 使用 VS Code 開啟資料夾並新增檔案：

    在終端機中輸入：

    ```bash
    code .
    ```

    這會用 VS Code 開啟當前的 Git 專案資料夾。

    在 VS Code 中，點選 **新增檔案**，建立一個名為 `hello.txt` 的檔案，並輸入以下內容：

    ```text
    Hello, Git!
    ```

    儲存檔案。

2. 回到終端機，查看目前的檔案狀態：

    ```bash
    git status
    ```

    你會看到 `hello.txt` 是未被追蹤的檔案。

3. 將檔案加入暫存區：

    ```bash
    git add hello.txt
    ```

4. 提交變更，建立一個儲存點：

    ```bash
    git commit -m "第一次提交：新增 hello.txt"
    ```

    > [!NOTE]
    > - `-m` 是指輸入一個提交訊息，讓未來的你或別人知道這次變更做了什麼。
    > - 提交訊息可以是英文、中文或任何語言，但是不能是空的。

5. 再次輸入 `git status`，你會發現工作區乾淨了，代表所有變更都已提交！

## 檢視與回顧你的版本紀錄

Git 幫你記錄了每一次的變更，你可以隨時查看或回到過去的版本。

輸入以下指令查看版本紀錄：

```bash
git log
```

你會看到每次提交的：

- **提交 ID**（一串長長的代碼）
- **作者**
- **日期**
- **提交訊息**

按 `q` 可以退出紀錄視窗。

## 修改檔案並提交第二次

讓我們再練習一次，這次來編輯 `hello.txt`！

1. 在 VS Code 中編輯 `hello.txt`：

    ```text
    This is my first Git project.
    ```

2. 查看變更：

    ```bash
    git status
    ```

    Git 會告訴你有檔案被修改了。

3. 檢視具體的變更內容：

    ```bash
    git diff
    ```

    你會看到舊的內容和新的內容的差異。

4. 重複 **add** 和 **commit** 的步驟：

    ```bash
    git add hello.txt
    git commit -m "修改 hello.txt，新增描述內容"
    ```

5. 再次輸入 `git log`，你會看到兩次提交的紀錄！

## 提交、分支與複製

在前面，我們學會了如何透過 `add` 和 `commit` 來儲存變更。這些步驟是 Git 最基本的操作，但隨著專案規模增大，你會發現有些功能十分必要：

1. **Branches**（分支）：讓你在不影響主線的情況下，嘗試新功能或修正錯誤。
2. **Forks**（複製分支）：讓你能夠在別人的專案上創建自己的版本，進行修改和貢獻。

### 什麼是 Branch？

在現實世界中，你可能會需要嘗試新功能或修正錯誤，這時候就可以使用 **Branch**（分支）。

- **主分支**（main 或 master）：穩定且主要的版本。
- **功能分支**：開發新功能的分支。

使用分支可以讓你在不影響主線程式碼的情況下，開發和測試新功能。

#### 創建分支與切換

1. 建立並切換一個新分支，並命名為 `feature`：

   ```bash
   git switch -c feature
   ```

2. 確認目前所在的分支：

   ```bash
   git branch
   ```

   目前所在的分支會有 `*` 標示。

3. 進行修改並提交：

   ```bash
   echo "New feature content" >> feature.txt
   git add feature.txt
   git commit -m "在 feature-branch 分支新增內容"
   ```

4. 切換回主分支：

   ```bash
   git checkout main
   ```

### 合併分支

當你完成了功能開發，想將分支的內容合併回主分支，可以使用 `merge`：

1. 切換到主分支：

   ```bash
   git checkout main
   ```

2. 合併分支：

   ```bash
   git merge feature-branch
   ```

3. 查看合併後的狀態：

   ```bash
   git log
   ```

### Fork 是什麼？

**Fork** 是在別人的 GitHub 倉庫中創建一個獨立的副本，讓你可以進行修改而不影響原倉庫。

- 在 GitHub 上按下 **Fork** 鈕，即可將倉庫複製到自己的帳號。
- Fork 適合開源專案，因為你可以安全地提出修改建議。

---

## 複製、編輯與發佈變更到 GitHub

這一部分，我們將實際操作如何使用 GitHub 進行合作，包括 **Fork**、**Clone**、**Edit** 和 **Pull Request** 的完整流程。

### Fork 和 Clone 一個現有的倉庫

1. **Fork 倉庫**

   - 打開你想要修改的 GitHub 倉庫，點選右上角的 **Fork**。
   - GitHub 會將該倉庫複製到你的帳號下。

2. **Clone 倉庫**

   - 在你的 GitHub 倉庫頁面，點擊 **Code**，複製 URL。
   - 在本機終端機輸入：
     ```bash
     git clone <倉庫的 URL>
     ```
       - 例如：
     ```bash
     git clone https://github.com/你的帳號/倉庫名稱.git
     ```

3. 進入倉庫資料夾：

   ```bash
   cd 倉庫名稱
   ```

### 創建分支並進行修改

1. 創建新分支：
   ```bash
   git checkout -b update-feature
   ```
2. 編輯檔案，例如新增一行內容到 `README.md`：
   ```text
   This is my contribution to the project!
   ```
3. 查看修改狀態：
   ```bash
   git status
   ```
4. 將修改加入暫存區並提交：
   ```bash
   git add README.md
   git commit -m "更新 README.md，新增我的貢獻內容"
   ```

### 推送分支到 GitHub

1. 將分支推送到遠端倉庫：
   ```bash
   git push origin update-feature
   ```
2. 打開 GitHub，你會看到新的分支出現在你的倉庫中。

### 開啟 Pull Request（PR）

1. 前往你的 GitHub 倉庫，點擊 **Pull Requests** > **New Pull Request**。
2. 選擇你剛才推送的分支（例如 `update-feature`）。
3. 填寫 PR 標題和描述，說明你的修改內容和目的。
4. 提交 PR。

這樣，你的修改就可以提交給原始倉庫的維護者進行審查，若被接受，將會合併到原始專案！

---

## 總結

透過這堂課程，我們學會了：

- **Branches**：如何創建分支，獨立進行功能開發。
- **Forks** 和 **Clone**：如何安全地複製並貢獻到開源專案。
- **Pull Request**：如何提出修改並合作開發。

多練習幾次，你會發現 Git 是一個非常強大的工具，讓你在開發中事半功倍！

---

現在輪到你了！

1. 找一個你喜歡的開源專案。
2. Fork 並 Clone 到本機。
3. 建立分支並嘗試修改檔案。
4. 提交 Pull Request，完成你的第一個開源貢獻！

加油！你離成為 Git 大師更近一步了！

yolo