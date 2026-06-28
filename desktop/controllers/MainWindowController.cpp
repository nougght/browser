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
    _coreAdapter->openInternalPage(InternalPageType::History, true);
}


void MainWindowController::onBookmarksClicked()
{
    _coreAdapter->openInternalPage(InternalPageType::Bookmarks, true);
}
