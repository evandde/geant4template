#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <fstream>

class G4Event;

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction() override;

    virtual void EndOfEventAction(const G4Event *) override;

private:
    G4int fHCID;
    static std::ofstream ofs;
};

#endif
