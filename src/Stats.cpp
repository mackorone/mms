#include "Stats.h"

#include <limits>

namespace mms {

Stats::Stats() : startedRun(false), solved(false), penalty(0.0) {}

void Stats::reset(StatsEnum stat) {
  setStat(stat, 0);
  updateScore();
}

void Stats::resetAll() {
  startedRun = false;
  solved = false;
  QMapIterator<StatsEnum, QLineEdit *> i(textField);
  while (i.hasNext()) {
    i.next();
    StatsEnum key = i.key();
    // Set best run equal to max value as a placeholder
    // Display no value until a start-to-finish run is recorded
    if (key == StatsEnum::BEST_RUN_TURNS) {
      statValues[key] = std::numeric_limits<float>::max();
      textField[key]->setText("");
    } else if (key == StatsEnum::BEST_RUN_DISTANCE) {
      statValues[key] = 0;
      textField[key]->setText("");
    } else if (key == StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE) {
      statValues[key] = 0;
      textField[key]->setText("");
    } else if (key == StatsEnum::SCORE) {
      // Score is set in updateScore()
      continue;
    } else {
      // Display zero for all other values
      setStat(key, 0);
    }
  }
  updateScore();
}

void Stats::addDistance(int distance) {
  float effectiveDistance = getEffectiveDistance(distance);
  increment(StatsEnum::TOTAL_DISTANCE, distance);
  increment(StatsEnum::TOTAL_EFFECTIVE_DISTANCE, effectiveDistance);
  if (startedRun) {
    increment(StatsEnum::CURRENT_RUN_DISTANCE, distance);
    increment(StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE, effectiveDistance);
  }
  updateScore();
}

void Stats::addTurn() {
  increment(StatsEnum::TOTAL_TURNS, 1);
  if (startedRun) {
    increment(StatsEnum::CURRENT_RUN_TURNS, 1);
  }
  updateScore();
}

void Stats::increment(StatsEnum stat, float increase) {
  setStat(stat, statValues[stat] + increase);
}

void Stats::setStat(StatsEnum stat, float value) {
  statValues[stat] = value;
  textField[stat]->setText(QString::number(statValues[stat]));
}

void Stats::bindText(StatsEnum stat, QLineEdit *uiText) {
  textField[stat] = uiText;
}

void Stats::updateScore() {
  // if solved, score = best_run_turns + best_run_effective_distance +
  // 0.1*(total_turns + total_effective_distance)
  float score;
  if (solved) {
    score = statValues[StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE] +
            statValues[StatsEnum::BEST_RUN_TURNS] +
            0.1 * (statValues[StatsEnum::TOTAL_EFFECTIVE_DISTANCE] +
                   statValues[StatsEnum::TOTAL_TURNS]);
  } else {
    score = 2000;  // default score
  }
  textField[StatsEnum::SCORE]->setText(QString::number(score));
}

float Stats::getEffectiveDistance(int distance) {
  return distance > 2 ? distance / 2.0 + 1 : distance;
}

void Stats::startRun() {
  reset(StatsEnum::CURRENT_RUN_TURNS);
  reset(StatsEnum::CURRENT_RUN_DISTANCE);
  reset(StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE);
  // add penalty to next run if necessary
  increment(StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE, penalty);
  increment(StatsEnum::TOTAL_EFFECTIVE_DISTANCE, penalty);
  penalty = 0;
  startedRun = true;
}

void Stats::finishRun() {
  startedRun = false;
  solved = true;
  float currentScore = statValues[StatsEnum::CURRENT_RUN_TURNS] +
                       statValues[StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE];
  float bestScore = statValues[StatsEnum::BEST_RUN_TURNS] +
                    statValues[StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE];
  if (currentScore < bestScore) {
    // new best run
    setStat(StatsEnum::BEST_RUN_TURNS,
            statValues[StatsEnum::CURRENT_RUN_TURNS]);
    setStat(StatsEnum::BEST_RUN_DISTANCE,
            statValues[StatsEnum::CURRENT_RUN_DISTANCE]);
    setStat(StatsEnum::BEST_RUN_EFFECTIVE_DISTANCE,
            statValues[StatsEnum::CURRENT_RUN_EFFECTIVE_DISTANCE]);
  }
  updateScore();
}

void Stats::endUnfinishedRun() {
  startedRun = false;
  updateScore();
}

void Stats::penalizeForReset() { penalty = 15; }

bool Stats::isInteger(StatsEnum stat) {
  // Returns true if the stat represents an integer value
  return (stat == StatsEnum::TOTAL_DISTANCE || stat == StatsEnum::TOTAL_TURNS ||
          stat == StatsEnum::BEST_RUN_DISTANCE ||
          stat == StatsEnum::BEST_RUN_TURNS ||
          stat == StatsEnum::CURRENT_RUN_DISTANCE ||
          stat == StatsEnum::CURRENT_RUN_TURNS);
}

QString Stats::getStat(StatsEnum stat) {
  QString statText = textField[stat]->text();
  // Cast the stat to an integer if it's supposed to be an integer
  if (isInteger(stat)) {
    bool converted;
    statText = QString::number(statText.toInt(&converted));
    if (converted) {
      return statText;
    } else {
      // Return empty string if integer conversion failed
      return "";
    }
  }
  return statText;
}

}  // namespace mms
