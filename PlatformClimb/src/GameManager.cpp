#include "GameManager.h"

GameManager manager;

std::wstring to_wstring(int value);
INT_PTR CALLBACK GameStartDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GameEndDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

GameManager::GameManager(){}

GameManager::~GameManager()
{
    FreeLibrary(hInstLib);
    std::cout << "Game manager destroyed." << std::endl;
}

void GameManager::Init(HWND hwnd)
{
    srand( GetTickCount() );

    hInstLib = LoadLibrary("..\\PlatformClimbDLL\\bin\\Debug\\PlatformClimbDLL.dll");
    if (hInstLib != NULL) {
        std::cout << "Loading functions." << std::endl;
        SetScore = (SETSCORE) GetProcAddress(hInstLib, "EnterLeaderboardRecord");
        GetPersonalBest = (GETPERSONAL) GetProcAddress(hInstLib, "GetPersonalBest");
        GetHighscore = (GETBEST) GetProcAddress(hInstLib, "GetHighscore");
        std::cout << "Functions loaded." << std::endl;
    }

    character.Init(hwnd);
    Platform* ground = new Platform(200, WINDOW_WIDTH + 40);
    ground->Init(hwnd);
    ground->Place(-20, WINDOW_HEIGHT - 150);
    platforms.push_back(ground);
}

void GameManager::ShowDialog(HWND hwnd) {
    int ret = DialogBox(NULL, MAKEINTRESOURCE(GAME_START_DIALOG), hwnd, GameStartDialogProc);
    ready = true;
}

void GameManager::Redraw(HWND hwnd)
{
    Bitmap buffer(WINDOW_WIDTH, WINDOW_HEIGHT, PixelFormat32bppARGB);

    Graphics graphics(&buffer);
    graphics.Clear(Color(100, 200, 255));

    cameraY += (cameraYGoal - cameraY) * 0.1;
    graphics.TranslateTransform(0, -cameraY);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    Graphics screenGraphics(hdc);

    if (!gameEndDialogOpen && gameStarted) {
        UpdateScore();
        CheckForLoss(hwnd);
        UpdatePlatforms(hwnd);

        character.Redraw(graphics, hwnd);

        for (int i = 0; i < platforms.size(); i++) {
            platforms[i]->Redraw(graphics);
            if (character.speedY > 0
                && platforms[i]->CheckCollision(character.x, character.y, character.charSize / 2)
            ) {
                character.NotifyAboutCollision();
                cameraYGoal = (platforms[i]->y - WINDOW_HEIGHT) + 150;
            }
        }

        FontFamily fontFamily(L"Arial");
        Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
        SolidBrush brush(Color(255, 255, 255));

        std::wstring scoreString = L"Score: " + to_wstring(score);
        std::wstring personalBestString = L"Personal best: " + to_wstring(personalBest);
        std::wstring highscoreString = L"Highscore: " + to_wstring(highscore);

        graphics.DrawString(highscoreString.c_str(), -1, &font, PointF(10.0, cameraY + 10.0), &brush);
        graphics.DrawString(personalBestString.c_str(), -1, &font, PointF(10.0, cameraY + 40.0), &brush);
        graphics.DrawString(scoreString.c_str(), -1, &font, PointF(10.0, cameraY + 70.0), &brush);
    }

    screenGraphics.DrawImage(&buffer, 0, 0);

    EndPaint(hwnd, &ps);
}

bool GameManager::Ready()
{
    return character.ready && ready;
}

void GameManager::StartGame(std::string name)
{
    highscore = GetHighscore();
    personalBest = GetPersonalBest(name);

    manager.playerName = name;
    manager.character.Reset();
    manager.gameStarted = true;
}

void GameManager::RestartGame(HWND hwnd)
{
    platforms.clear();
    score = 0;
    cameraY = 0;
    cameraYGoal = 0;
    Platform* ground = new Platform(200, WINDOW_WIDTH + 40);
    ground->Init(hwnd);
    ground->Place(-20, WINDOW_HEIGHT - 150);
    platforms.push_back(ground);
    character.Reset();
}

void GameManager::UpdateScore()
{
    score = std::max(-cameraY / 100, score);
    personalBest = std::max(personalBest, score);
    highscore = std::max(highscore, score);
}

void GameManager::CheckForLoss(HWND hwnd)
{
    if (!gameEndDialogOpen && character.speedY > 30) {
        gameEndDialogOpen = true;
        SetScore(playerName, score);
        DialogBox(NULL, MAKEINTRESOURCE(GAME_END_DIALOG), hwnd, GameEndDialogProc);
    }
}

void GameManager::UpdatePlatforms(HWND hwnd)
{
    for (int i = 0; i < platforms.size(); i++) {
        if (platforms[i]->y > cameraY + 2 * WINDOW_HEIGHT) {
            delete platforms[i];
            platforms.erase(platforms.begin() + i);
            i--;
        }
    }

    for (int i = platforms.size(); i < platformCount; i++) {
        Platform* temp = new Platform(platformHeight, platformWidth);
        temp->Place(
            rand() % (WINDOW_WIDTH - platformWidth),
            platforms[platforms.size() - 1]->y - platformGap
        );
        platforms.push_back(temp);
        platforms[platforms.size() - 1]->Init(hwnd);
    }
}

std::wstring to_wstring(int value)
{
    std::wstringstream tempss;
    tempss << value;
    return tempss.str();
}

INT_PTR CALLBACK GameStartDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG:
        {
            SetWindowPos(hDlg, HWND_TOP, 200, 200, 825, 500, SWP_NOZORDER | SWP_SHOWWINDOW);
            std::cout << "Start dialog created." << std::endl;
        }
        return true;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case START_BUTTON_DIALOG:
            {
                HWND hEdit = GetDlgItem(hDlg, DIALOG_NAME_TEXT);
                int length = GetWindowTextLength(hEdit);
                LPSTR buffer = new TCHAR[length + 1];
                GetWindowText(hEdit, buffer, length + 1);
                EndDialog(hDlg, 0);
                manager.StartGame(std::string(buffer));
                std::cout << "Game started." << std::endl;
            }
            return TRUE;
        }
        return true;
    case WM_CLOSE:
        {
            EndDialog(hDlg, 0);
        }
        return TRUE;
    case WM_DESTROY:
        EndDialog(hDlg, 0);
        return TRUE;
    }
    return FALSE;
}

INT_PTR CALLBACK GameEndDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_INITDIALOG:
        {
            std::string scoreString = "Score: " + std::to_string(manager.score);
            SetDlgItemText(hDlg, DIALOG_SCORE_TEXT, scoreString.c_str());
        }
        return true;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case RETRY_BUTTON_DIALOG:
            {
                manager.RestartGame(GetParent(hDlg));
                EndDialog(hDlg, 0);
                manager.gameEndDialogOpen = false;
            }
            return TRUE;
        }
        return true;
    case WM_CLOSE:
        {
            EndDialog(hDlg, 0);
        }
        return TRUE;
    case WM_DESTROY:
        EndDialog(hDlg, 0);
        return TRUE;
    }
    return FALSE;
}
