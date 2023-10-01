// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright (c) 2023, Lev Leontev

#include "SettingsDialog.h"

#include <fmt/core.h>
#include <obs-module.h>
#include <obs.h>

#include <iostream>
#include <obs.hpp>

#include "Log.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(RewardsTheaterPlugin& plugin, QWidget* parent)
    : QDialog(parent), plugin(plugin), ui(std::make_unique<Ui::SettingsDialog>()),
      authenticateWithTwitchDialog(new AuthenticateWithTwitchDialog(this, plugin.getTwitchAuth())) {
    ui->setupUi(this);
    setFixedSize(size());

    connect(ui->authButton, &QPushButton::clicked, this, &SettingsDialog::logInOrLogOut);
    connect(ui->openRewardsQueueButton, &QPushButton::clicked, this, &SettingsDialog::openRewardsQueue);
    connect(&plugin.getTwitchAuth(), &TwitchAuth::onUsernameChanged, this, &SettingsDialog::updateAuthButtonText);
    connect(&plugin.getTwitchRewardsApi(), &TwitchRewardsApi::onRewardsUpdated, this, &SettingsDialog::showRewards);

    plugin.getTwitchRewardsApi().startService();
}

void SettingsDialog::logInOrLogOut() {
    TwitchAuth& auth = plugin.getTwitchAuth();
    if (auth.isAuthenticated()) {
        auth.logOut();
    } else {
        authenticateWithTwitchDialog->show();
    }
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::openRewardsQueue() {
    log(LOG_INFO, "onOpenRewardsQueueClicked");
}

void SettingsDialog::updateAuthButtonText(const std::optional<std::string>& username) {
    std::string newText;
    if (plugin.getTwitchAuth().isAuthenticated()) {
        std::string usernameShown;
        if (username) {
            usernameShown = username.value();
        } else {
            usernameShown = obs_module_text("ErrorUsername");
        }
        newText = fmt::format(fmt::runtime(obs_module_text("LogOut")), usernameShown);
    } else {
        newText = obs_module_text("LogIn");
    }
    ui->authButton->setText(QString::fromStdString(newText));
}

void SettingsDialog::showRewards(const std::vector<Reward>& rewards) {
    log(LOG_INFO, "Rewards count: {}", rewards.size());
}
