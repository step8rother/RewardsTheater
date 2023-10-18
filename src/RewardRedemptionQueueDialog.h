// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2023, Lev Leontev

#pragma once

#include <QDialog>
#include <QWidget>
#include <memory>

#include "Reward.h"
#include "RewardRedemptionQueue.h"
#include "RewardRedemptionQueueDialog.h"

namespace Ui {
class RewardRedemptionQueueDialog;
}

class RewardRedemptionQueueDialog : public QDialog {
    Q_OBJECT

public:
    RewardRedemptionQueueDialog(RewardRedemptionQueue& rewardRedemptionQueue, QWidget* parent);
    ~RewardRedemptionQueueDialog();

private slots:
    void showRewardRedemptions(const std::vector<RewardRedemption>& rewardRedemptionQueue);

private:
    RewardRedemptionQueue& rewardRedemptionQueue;
    std::unique_ptr<Ui::RewardRedemptionQueueDialog> ui;
};
