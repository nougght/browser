#include "MainWindowController.h"


MainWindowController::MainWindowController(CoreAdapter * coreAdapter)
    : _coreAdapter(coreAdapter)
{
    _setupEvents();
}

void MainWindowController::_setupEvents()
{
    
    
}


void MainWindowController::onHistoryClicked()
{
    emit historyPageRequested();
}


void MainWindowController::onBookmarksClicked()
{
    emit bookmarksPageRequested();
}
