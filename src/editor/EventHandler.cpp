#include "common.h"
#include "EditorEngine.h"
#include "AppFalcon.h"
#include "TileLayer.h"
#include "FileDialog.h"
#include "TileboxPanel.h"
#include "DrawAreaPanel.h"


void EditorEngine::action(const gcn::ActionEvent& ae)
{
}

void EditorEngine::keyPressed(gcn::KeyEvent& ke)
{
    switch(ke.getKey().getValue())
    {
    case 't': case 'T':
        ToggleTileWnd();
        break;

    case 'g': case 'G':
        ToggleTilebox();
        break;

    case 'p': case 'P':
        ToggleSelPreviewLayer();
        break;

    case 'l': case 'L':
        ToggleLayerPanel();
        break;

    case 'm': case 'M':
        panMain->SetActiveLayer((panMain->GetActiveLayerId() + 1) % LAYER_MAX);
        break;

    case 'n': case 'N':
        panMain->SetActiveLayer((panMain->GetActiveLayerId() + LAYER_MAX - 1) % LAYER_MAX);
        break;

    case 's': case 'S':
        if(ke.isControlPressed())
        {
            if(ke.isShiftPressed())
                _fileDlg->Open(true, "map");
            else
                _SaveCurrentMap();
        }
        break;

    case 'o': case 'O':
        if(ke.isControlPressed())
            _fileDlg->Open(false, "map");
        break;

        // TODO: below, fix for tile size != 16
    case gcn::Key::UP:
        PanDrawingArea(0, -16 * (ke.isControlPressed() ? 5 : 1) );
        break;

    case gcn::Key::DOWN:
        PanDrawingArea(0, 16 * (ke.isControlPressed() ? 5 : 1) );
        break;

    case gcn::Key::LEFT:
        PanDrawingArea(-16 * (ke.isControlPressed() ? 5 : 1) , 0);
        break;

    case gcn::Key::RIGHT:
        PanDrawingArea(16 * (ke.isControlPressed() ? 5 : 1) , 0);
        break;
    }
}

void EditorEngine::mousePressed(gcn::MouseEvent& me)
{
    gcn::Widget *src = me.getSource();

    if(me.getButton() == gcn::MouseEvent::RIGHT)
    {
        if(src == panMain)
        {
            _mouseRightStartX = me.getX();
            _mouseRightStartY = me.getY();
        }
    }

    // TODO: clean up this mess, this must be possible to do easier!
    if(src == panMain && me.getButton() == gcn::MouseEvent::LEFT)
    {
        //HandlePaintOnWidget(src, me.getX(), me.getY(), true);
    }
    else if(src == panTilebox && me.getButton() == gcn::MouseEvent::RIGHT)
    {
        //HandlePaintOnWidget(src, me.getX(), me.getY(), false);
    }
}

void EditorEngine::mouseDragged(gcn::MouseEvent& me)
{
    gcn::Widget *src = me.getSource();

    if(src == panMain)
    {
        if(me.getButton() == gcn::MouseEvent::LEFT) // paint on left-click
        {
            //HandlePaintOnWidget(src, me.getX(), me.getY(), true);
        }
        else if(me.getButton() == gcn::MouseEvent::RIGHT) // pan on right-click
        {
            int32 xdiff = _mouseRightStartX - me.getX();
            int32 ydiff = _mouseRightStartY - me.getY();
            _mouseRightStartX = me.getX();
            _mouseRightStartY = me.getY();
            uint32 multi = me.isControlPressed() ? 5 : 1;
            PanDrawingArea(xdiff * multi, ydiff * multi);
        }
    }
}

void EditorEngine::mouseWheelMovedDown(gcn::MouseEvent& me)
{
    mouseWheelMoved(me, false);
}

void EditorEngine::mouseWheelMovedUp(gcn::MouseEvent& me)
{
    mouseWheelMoved(me, true);
}

void EditorEngine::mouseWheelMoved(gcn::MouseEvent& me, bool up)
{
    gcn::Widget *src = me.getSource();
    if(src == panMain)
    {
        int32 pan = (me.isControlPressed() ? 5*3 : 3) * (up ? -16 : 16);
        if(me.isShiftPressed())
            PanDrawingArea(pan, 0);
        else
            PanDrawingArea(0, pan);
    }
}

void EditorEngine::FileChosenCallback(FileDialog *dlg)
{
    DEBUG(ASSERT(dlg == _fileDlg)); // just to be sure it works, there is only one FileDialog object anyways

    logdebug("FileChosenCallback, operation '%s'", dlg->GetOperation());

    // Load/Save map file
    if(!strcmp(dlg->GetOperation(), "map"))
    {
        std::string fn = dlg->GetFileName();
        Falcon::VMachine *vm = falcon->GetVM();
        if(dlg->IsSave())
        {
            Falcon::Item *item = vm->findGlobalItem("OnSaveMap");
            if(item && item->isCallable())
            {
                try
                {
                    vm->pushParam(new Falcon::CoreString(fn.c_str()));
                    vm->callItem(*item, 1);
                    if(!vm->regA().isTrue())
                    {
                        logerror("OnSaveMap: Saving aborted");
                        return;
                    }
                    _SaveCurrentMapAs(fn.c_str());
                }
                catch(Falcon::Error *err)
                {
                    Falcon::AutoCString edesc( err->toString() );
                    logerror("OnSaveMap: %s", edesc.c_str());
                    err->decref();
                }
            }
            else
            {
                logerror("WARNING: OnSaveMap() not present or not callable, saving map directly. Additional map data may be lost!");
                _SaveCurrentMapAs(fn.c_str()); // FALLBACK
            }
        }
        else
        {
            Falcon::Item *item = vm->findGlobalItem("OnLoadMap");
            if(item && item->isCallable())
            {
                try
                {
                    vm->pushParam(new Falcon::CoreString(fn.c_str()));
                    vm->callItem(*item, 1);
                    if(!vm->regA().isTrue())
                    {
                        logerror("'%s' can't be loaded, invalid?", fn.c_str());
                        return; // keep file dialog open
                    }
                }
                catch(Falcon::Error *err)
                {
                    Falcon::AutoCString edesc( err->toString() );
                    logerror("OnLoadMap: %s", edesc.c_str());
                    err->decref();
                }
            }
            else
            {
                logerror("WARNING: OnLoadMap() not present or not callable, loading map directly. Additional map data may be lost if saved!");
                LoadMapFile(fn.c_str()); // FALLBACK
            }
        }
    }

    dlg->Close();

}
