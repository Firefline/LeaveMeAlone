// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "LMAAIController.h"
#include "LeaveMeAlone/Enemy/LMAEnemyCharacter.h"

void ALMAAIController::OnPossess(APawn *InPawn) 
{
  Super::OnPossess(InPawn);
  const auto AICharacter = Cast<ALMAEnemyCharacter>(InPawn);
  if (AICharacter) 
  {
    RunBehaviorTree(AICharacter->BehaviorTreeAsset);

  }

}
