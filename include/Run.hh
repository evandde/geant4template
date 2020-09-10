#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"

class Run : public G4Run
{
public:
    Run();
    ~Run() override;

    virtual void RecordEvent(const G4Event *) override;
    virtual void Merge(const G4Run *) override;

    G4double GetEDep() const { return fEDep; }

private:
    G4int fEDepHCID;
    G4double fEDep;
};

#endif // RUN_HH
