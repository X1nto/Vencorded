#include <string>
#include "git2.h"
#include "../directories/vencorded.h"
#include "downloader.h"
#include <filesystem>

#define GIT_SAFE(statement) err = statement; if (err != 0) goto cleanup;

bool vencorded::update_vencord()
{
    int err;

    git_repository* repo = nullptr;
    git_reference* head = nullptr;
    git_reference* upstream = nullptr;
    git_annotated_commit* upstream_ref = nullptr;
    git_rebase* rebase = nullptr;
    git_rebase_operation* rebase_op = nullptr;
    git_oid rebase_oid = {{0}};

    const git_rebase_options rebase_opts = GIT_REBASE_OPTIONS_INIT;

    const std::string repo_dir = std::filesystem::canonical(vencorded::vencorded_vencord_path()).string();
    GIT_SAFE(git_repository_open(&repo, repo_dir.c_str()))
    GIT_SAFE(git_repository_head(&head, repo))
    GIT_SAFE(git_branch_upstream(&upstream, head))
    GIT_SAFE(git_annotated_commit_from_ref(&upstream_ref, repo, head))
    GIT_SAFE(git_rebase_init(&rebase, repo, nullptr, upstream_ref, nullptr, &rebase_opts))

    while (git_rebase_next(&rebase_op, rebase) == 0)
    {
        GIT_SAFE(git_rebase_commit(&rebase_oid, rebase, nullptr, nullptr, nullptr, nullptr))
    };

    GIT_SAFE(git_rebase_finish(rebase, nullptr));

cleanup:
    git_repository_free(repo);
    git_reference_free(head);
    git_reference_free(upstream);
    git_annotated_commit_free(upstream_ref);
    git_rebase_free(rebase);

    return err == 0;
}

bool vencorded::download_vencord()
{
    git_repository* repo = nullptr;

    int err;

    git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
    const std::filesystem::path repo_path = vencorded::vencorded_vencord_path();
    if (std::filesystem::exists(repo_path))
    {
        std::filesystem::remove_all(repo_path);
    }
    std::filesystem::create_directories(repo_path);
    const std::string repo_dir = std::filesystem::canonical(repo_path).string();
    //TODO add depth options once https://github.com/libgit2/libgit2/pull/6396 gets merged
    err = git_clone(
        &repo,
        "https://github.com/Vendicated/Vencord",
        repo_dir.c_str(),
        &clone_opts
    );

    git_repository_free(repo);
    
    return err == 0;
}

void vencorded::nuke_vencord()
{
    std::filesystem::remove_all(vencorded::vencorded_vencord_path());
}

bool vencorded::vencord_downloaded()
{
    const std::filesystem::path repo_path = vencorded::vencorded_vencord_path();
    if (!std::filesystem::exists(repo_path))
    {
        return false;
    }

    git_repository* repo = nullptr;
    const std::string repo_dir = std::filesystem::canonical(repo_path).string();
    int status = git_repository_open_ext(&repo, repo_dir.c_str(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr);
    return status == 0;
}