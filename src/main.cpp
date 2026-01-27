#include <Geode/modify/LevelCell.hpp>
#include <Geode/modify/SongInfoLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/MusicBrowser.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

using namespace geode::prelude;

struct LogoProperties {
    float offsetY = 0.f;
    float extraHeight = 0.f;
};

static const std::unordered_map<std::string_view, LogoProperties> g_artists = {
    { "Camellia", LogoProperties{ .offsetY = -1.f } },
    { "bossfightofficial", LogoProperties{ .offsetY = -2.f, .extraHeight = 10.1f } },
    { "ThisIsTheFatRat", LogoProperties{ .offsetY = -2.f, .extraHeight = 10.1f } },
    { "Shirobon", LogoProperties{ .offsetY = -1.f, .extraHeight = 4.1f } },
    { "ParagonX9", LogoProperties{ .offsetY = -1.f, .extraHeight = -7.f } },
    { "t+pazolite", LogoProperties{ .offsetY = -2.f, .extraHeight = 4.f } },
    { "Rukkus", LogoProperties{ .offsetY = -2.f, .extraHeight = 8.f } },
    { "Envy", LogoProperties{ .offsetY = -1.f, .extraHeight = -1.f } },
    { "garlagan", LogoProperties{ .offsetY = -2.f, .extraHeight = 7.4f } },
    { "MDK", LogoProperties{ .offsetY = -2.f, .extraHeight = 12.f } },
    { "F-777", LogoProperties{ .extraHeight = 4.f } },
    { "LeaF", LogoProperties{ .offsetY = -1.5f, .extraHeight = 13.f } },
    { "ColBreakz", LogoProperties{ .offsetY = -1.f, .extraHeight = 8.5f } },
    { "Koraii", LogoProperties{ .offsetY = -1.5f, .extraHeight = 5.f } },
    { "BoomKitty", LogoProperties{ .offsetY = -1.5f, .extraHeight = 10.f } },
    { "lchavasse", LogoProperties{ .offsetY = -1.5f, .extraHeight = 6.7f } },
    { "OcularNebula", LogoProperties{ .offsetY = -1.5f, .extraHeight = 10.f } },
    { "Lockyn", LogoProperties{ .offsetY = -1.5f, .extraHeight = 9.f } },
    { "Dunderpatrullen", LogoProperties{ .offsetY = -1.5f, .extraHeight = 5.f } },
    { "dj-Nate", LogoProperties{ .offsetY = -1.9f, .extraHeight = 11.f } },
    { "ArdolfGD", LogoProperties{ .offsetY = -1.3f, .extraHeight = 9.f } },
    { "NightHawk22", LogoProperties{ .offsetY = -1.3f, .extraHeight = 6.f } },
    { "xi", LogoProperties{ .offsetY = -0.68f, .extraHeight = 7.f } },
    { "Teminite", LogoProperties{ .offsetY = -1.5f, .extraHeight = 7.f } },
    { "Xtrullor", LogoProperties{ .offsetY = -1.5f, .extraHeight = 11.f } },
    { "1f1n1ty", LogoProperties{ .offsetY = -1.5f, .extraHeight = 10.f } },
    { "SixImpala", LogoProperties{ .offsetY = -1.5f, .extraHeight = 7.f } },
    { "dexarson", LogoProperties{ .extraHeight = 13.f } },
    { "cysmix", LogoProperties{ .offsetY = -1.5f, .extraHeight = 9.f } },
    { "meganeko", LogoProperties{ .offsetY = -2.5f, .extraHeight = 7.f } },
    { "Helblinde", LogoProperties{ .offsetY = -1.5f, .extraHeight = 11.f } },
    { "hinkik", LogoProperties{ .offsetY = -1.4f, .extraHeight = -3.5f } },
    { "Acid-Notation", LogoProperties{ .offsetY = -1.5f } },
    { "DJVI", LogoProperties{ .offsetY = -1.5f, .extraHeight = 8.4f } },
    { "Step", LogoProperties{ .offsetY = -1.5f, .extraHeight = 7.f } },
    { "ForeverBound", LogoProperties{ .offsetY = -1.5f, .extraHeight = 3.5f } },
    { "Frums", LogoProperties{ .offsetY = -1.5f, .extraHeight = 7.f } },
    { "Dimrain47", LogoProperties{ .offsetY = -0.9f, .extraHeight = 3.f } },
    { "CreoMusic", LogoProperties{ .offsetY = -4.8f, .extraHeight = 10.f } },
    { "Waterflame", LogoProperties{ .offsetY = -2.f, .extraHeight = 6.f } },
    { "Extra Terra", LogoProperties{ .offsetY = -2.f, .extraHeight = 11.f } },
    { "TheLivingTombstone", LogoProperties{ .offsetY = -1.f, .extraHeight = 13.f } },
};

static const std::unordered_map<std::string_view, std::string_view> g_aliases {
    {"Cametek", "Camellia"},
    {"hyperdemented", "CreoMusic"},
    {"Boom Kitty", "BoomKitty"},
    {"TOMBOYY", "Koraii"},
    {"DJ-Nate", "dj-Nate"},
    {"shirobonmusic", "Shirobon"},
};

std::string_view getArtistFilename(std::string_view artist) {
    if (g_artists.contains(artist)) {
        return artist;
    }

    if (g_aliases.contains(artist)) {
        return g_aliases.at(artist);
    }

    return artist;
}

bool isArtistAdded(std::string_view artist) {
    if (!g_artists.contains(artist)) {
        return g_aliases.contains(artist);
    }

    return true;
}

float getOffsetY(std::string_view artist) {
    return g_artists.at(getArtistFilename(artist)).offsetY;
}

float getExtraHeight(std::string_view artist) {
    return g_artists.at(getArtistFilename(artist)).extraHeight;
}

class $modify(LevelCell) {

    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        if (m_mainLayer->getChildByID("ncs-icon")) {
            return;
        }

        auto songLabel = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("song-name"));

        if (!songLabel) {
            return;
        }

        std::string artist;

        auto object = static_cast<SongInfoObject*>(MusicDownloadManager::sharedState()->getSongInfoObject(m_level->m_songID));
        
        if (!object) {
            artist = LevelTools::nameForArtist(LevelTools::artistForAudio(m_level->m_audioTrack));
        } else {
            artist = object->m_artistName;
        }

        if (!isArtistAdded(artist)) {
            return;
        }
        auto spr = CCSprite::create(fmt::format("{}.png"_spr, getArtistFilename(artist)).c_str());
        spr->setScale(11.f / (spr->getContentHeight() - getExtraHeight(artist)));
        spr->setID("artist-logo"_spr);
        
        auto ogX = songLabel->getPositionX();
        auto ogScale = songLabel->getScale();

        songLabel->setPositionX(songLabel->getPositionX() + spr->getScaledContentWidth() + 4.25f);
        songLabel->limitLabelWidth((m_compactView ? 185.f : 195.f) - spr->getScaledContentWidth() - 4.25f, songLabel->getScale(), 0.f);

        if (m_compactView) {
            auto offset = songLabel->getPositionX() - ogX - (songLabel->getContentWidth() * ogScale - songLabel->getScaledContentWidth());
            
            if (auto coin = m_mainLayer->getChildByID("coin-icon-1")) {
                coin->setPositionX(coin->getPositionX() + offset);
            }

            if (auto coin = m_mainLayer->getChildByID("coin-icon-2")) {
                coin->setPositionX(coin->getPositionX() + offset);
            }

            if (auto coin = m_mainLayer->getChildByID("coin-icon-3")) {
                coin->setPositionX(coin->getPositionX() + offset);
            }
        }

        spr->setAnchorPoint({0, 0.5f});
        spr->setPosition({ogX, songLabel->getPositionY() + getOffsetY(artist) * 0.85f});

        m_mainLayer->addChild(spr);
    }

};

class $modify(SongInfoLayer) {

    bool init(gd::string songName, gd::string artistName, gd::string songURL, gd::string newgroundsURL, gd::string youtubeURL, gd::string facebookURL, int songID, gd::string songTags, int nongType) {
        if (!SongInfoLayer::init(songName, artistName, songURL, newgroundsURL, youtubeURL, facebookURL, songID, songTags, nongType)) {
            return false;
        }

        log::debug("{}", nongType);

        if (nongType == 1) { // ncs
            return true;
        }

        std::string artist = artistName;

        if (!isArtistAdded(artist)) {
            return true;
        }

        auto spr = CCSprite::create(fmt::format("{}.png"_spr, getArtistFilename(artist)).c_str());
        spr->setAnchorPoint({1.f, 1.f});
        spr->setPosition(CCDirector::get()->getWinSize() / 2.f + ccp(200.75f, 130.4f));
        spr->setScale(16.8f / (spr->getContentHeight() - getExtraHeight(artist)));
        spr->setID("artist-logo"_spr);

        m_mainLayer->addChild(spr);

        return true;
    }

};

class $modify(ProCustomSongWidget, CustomSongWidget) {

    struct Fields {
        CCSprite* m_logo = nullptr;
        CCPoint m_ogPosition = {0, 0};
    };
 
    static void onModify(auto& self) {
        (void)self.setHookPriorityAfterPost("CustomSongWidget::updateWithMultiAssets", "fleym.nongd");
        (void)self.setHookPriorityAfterPost("CustomSongWidget::updateSongInfo", "fleym.nongd");
    }

    void updateLogo() {
        if (!m_songInfoObject || m_songInfoObject->m_nongType == 1) { // ncs
            return;
        }

        std::string artist = m_songInfoObject->m_artistName;

        if (!isArtistAdded(artist)) {
            return;
        }

        auto f = m_fields.self();

        if (!f->m_logo) {
            f->m_logo = CCSprite::create(fmt::format("{}.png"_spr, getArtistFilename(artist)).c_str());
            f->m_logo->setAnchorPoint({0.f, 0.5f});
            f->m_logo->setScale(16.f / (f->m_logo->getContentHeight() - getExtraHeight(artist)));
            f->m_logo->setID("artist-logo"_spr);

            if (m_isMusicLibrary) {
                f->m_logo->setScale(f->m_logo->getScale() * 0.74f);
            }
            
            addChild(f->m_logo);

            f->m_ogPosition = m_songLabel->getPosition();
        }

        f->m_logo->setPosition(f->m_ogPosition + ccp(0, getOffsetY(artist) * (m_isMusicLibrary ? 1.f : 1.25f)));
        m_songLabel->setPositionX(f->m_logo->getPositionX() + f->m_logo->getScaledContentWidth() + 4.25f);

        auto prevScale = m_songLabel->getScale();
        auto width = 240.f - (f->m_logo->getScaledContentWidth() + 4.25f) + (m_infoBtn && m_infoBtn->isVisible() ? 20.f : 30.f);

        width -= m_playbackBtn && m_playbackBtn->isVisible() ? m_playbackBtn->getScaledContentWidth() : 0.f;

        if (m_isMusicLibrary) {
            width -= m_moreBtn->getNormalImage()->getScaledContentWidth();
            width -= m_downloadBtn && m_downloadBtn->isVisible() ? m_downloadBtn->getScaledContentWidth() : 0.f;
            width -= m_selectSongBtn && m_selectSongBtn->isVisible() ? m_selectSongBtn->getScaledContentWidth() : 0.f;
            width -= m_cancelDownloadBtn && m_cancelDownloadBtn->isVisible() ? m_cancelDownloadBtn->getScaledContentWidth() : 0.f;
        } 

        m_songLabel->limitLabelWidth(width, m_songLabel->getScale(), 0.f);

        if (m_isMusicLibrary) {
            m_moreBtn->setPositionX(m_moreBtn->getPositionX() - (m_songLabel->getContentWidth() * prevScale - m_songLabel->getScaledContentWidth()));
        }

        if (auto nong = Loader::get()->getLoadedMod("fleym.nongd")) {
            if (auto menu = getChildByID("fleym.nongd/nong-menu")) {
                if (!m_isMusicLibrary) {
                    if (!nong->getSettingValue<bool>("old-label-display")) {
                        menu->setPosition(f->m_ogPosition + ccp(-9.f, 13.f));
                    } else {
                        menu->setPosition(m_songLabel->getPosition());
                    }
                }
            }
        }
    }

    void positionInfoObjects() {
        CustomSongWidget::positionInfoObjects();
        updateLogo();
    }

    void updateWithMultiAssets(gd::string p0, gd::string p1, int p2) {
        CustomSongWidget::updateWithMultiAssets(p0, p1, p2);
        updateLogo();
    }

    void updateSongInfo() {
        CustomSongWidget::updateSongInfo();
        updateLogo();
    }

};

class $modify(MusicBrowser) {

    void onArtistFilters(CCObject* sender) {
        MusicBrowser::onArtistFilters(sender);

        auto layer = CCScene::get()->getChildByType<OptionsScrollLayer>(0);

        if (!layer) {
            return;
        }

        for (auto cell : layer->m_listLayer->m_list->m_tableView->m_contentLayer->getChildrenExt<OptionsCell*>()) {
            std::string artist = cell->m_optionsObject->m_name;
            
            if (!isArtistAdded(artist)) {
                continue;
            }

            auto lbl = cell->m_mainLayer->getChildByType<CCLabelBMFont>(0);

            if (!lbl) {
                continue;
            }

            auto logo = CCSprite::create(fmt::format("{}.png"_spr, getArtistFilename(artist)).c_str());
            logo->setAnchorPoint({0.f, 0.5f});
            logo->setScale(12.f / (logo->getContentHeight() - getExtraHeight(artist)));
            logo->setScale(logo->getScale() * 0.74f);
            logo->setPosition(lbl->getPosition() + ccp(0, getOffsetY(artist) * 1.25f));
            logo->setID("artist-logo"_spr);
            
            cell->m_mainLayer->addChild(logo);

            auto prevX = lbl->getPositionX();

            lbl->setPositionX(logo->getPositionX() + logo->getScaledContentWidth() + 4.25f);

            if (auto lbl2 = cell->m_mainLayer->getChildByType<CCLabelBMFont>(1)) {
                lbl2->setPositionX(lbl2->getPositionX() + lbl->getPositionX() - prevX);
            }
        }
    }

};

class $modify(LevelSelectLayer) {

    void onDownload(CCObject* sender) {
        LevelSelectLayer::onDownload(sender);

        auto layer = CCScene::get()->getChildByType<SongsLayer>(0);

        if (!layer) {
            return;
        }

        auto listLayer = layer->getChildByIndex(0)->getChildByType<GJListLayer>(0);
        
        for (auto cell : listLayer->getChildByType<CustomListView>(0)->m_tableView->m_contentLayer->getChildrenExt<SongCell*>()) {
            std::string artist = LevelTools::nameForArtist(LevelTools::artistForAudio(cell->m_songObject->m_audioID));
            
            if (!isArtistAdded(artist)) {
                continue;
            }

            auto lbl = cell->m_mainLayer->getChildByType<CCLabelBMFont>(0);

            if (!lbl) {
                continue;
            }

            auto logo = CCSprite::create(fmt::format("{}.png"_spr, getArtistFilename(artist)).c_str());
            logo->setAnchorPoint({0.f, 0.5f});
            logo->setScale(16.25f / (logo->getContentHeight() - getExtraHeight(artist)));
            logo->setScale(logo->getScale() * 0.74f);
            logo->setPosition(lbl->getPosition() + ccp(0, getOffsetY(artist) * 1.25f));
            logo->setID("artist-logo"_spr);
            
            cell->m_mainLayer->addChild(logo);

            auto prevX = lbl->getPositionX();

            lbl->setPositionX(logo->getPositionX() + logo->getScaledContentWidth() + 4.25f);
            lbl->limitLabelWidth(222.f, lbl->getScale(), 0.f);
        }
    }

};