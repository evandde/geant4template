#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"

class Run : public G4Run
{
public:
    Run() = default;
    ~Run() override = default;

    void RecordEvent(const G4Event*) override;
    void Merge(const G4Run*) override;

private:
    G4int fHCID = -1;
};

#endif
