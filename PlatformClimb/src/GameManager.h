#include <vector>
#include <sstream>

#include "Definitions.h"
#include "Dialogs/GameStartDialog.h"
#include "Dialogs/GameEndDialog.h"
#include "Character.h"
#include "Platform.h"

typedef void (_cdecl *SETSCORE) (std::string, int);
typedef int (_cdecl *GETPERSONAL) (std::string);
typedef int (_cdecl *GETBEST) ();

class GameManager {
public:
    GameManager();
    ~GameManager();

    void Init(HWND hwnd);
    void ShowDialog(HWND hwnd);
    void Redraw(HWND hwnd);
    bool Ready();
    void StartGame(std::string name);
    void RestartGame(HWND hwnd);

    bool gameStarted = false;
    bool gameEndDialogOpen = false;
    int score;

private:
    void UpdateScore();
    void CheckForLoss(HWND hwnd);
    void UpdatePlatforms(HWND hwnd);

    int cameraYGoal = 0;
    int cameraY = 0;
    bool ready = false;

    int personalBest = 0;
    int highscore = 0;

    int platformGap = 235;
    int platformCount = 10;
    const int platformHeight = 30, platformWidth = 150;
    std::vector<Platform*> platforms;

    std::string playerName;
    Character character;

    HINSTANCE hInstLib;
    SETSCORE SetScore;
    GETPERSONAL GetPersonalBest;
    GETBEST GetHighscore;
};

extern GameManager manager;
