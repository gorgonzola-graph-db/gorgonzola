#!/bin/bash
OLD_HEAD=$(git rev-parse master)
git checkout -B temp-squash b6ed78e

# Function to abort on error
die() {
    echo "$1"
    git cherry-pick --abort 2>/dev/null || true
    git checkout master
    git branch -D temp-squash
    exit 1
}

echo "Squashing Group 1..."
git cherry-pick 322d128 0c65d87 c64fbd4 8c0ce02 6f51005 || die "Failed cherry-pick Group 1"
git reset --soft b6ed78e
git commit --author="lechibang-1512 <176822289+lechibang-1512@users.noreply.github.com>" --date="2026-07-22T16:44:41+07:00" -m "refactor: consolidate tools and benchmark directories"

echo "Picking interleaved commits..."
git cherry-pick 4d91c25 || die "Failed cherry-pick 4d91c25"
git cherry-pick c2039b4 || die "Failed cherry-pick c2039b4"

echo "Squashing Group 2..."
# Save current HEAD before picking Group 2 commits
GROUP2_BASE=$(git rev-parse HEAD)
git cherry-pick b41feeb 10fb99e 5c8ed2f 40953b0 || die "Failed cherry-pick Group 2"
git reset --soft $GROUP2_BASE
git commit --author="lechibang-1512 <176822289+lechibang-1512@users.noreply.github.com>" --date="2026-07-22T17:13:13+07:00" -m "refactor: clean up core declarations, fix CI paths, and apply formatting"

echo "Picking remaining commits..."
git cherry-pick 82ce404 || die "Failed cherry-pick 82ce404"
git cherry-pick f60fb24 || die "Failed cherry-pick f60fb24"

echo "Successfully squashed. Updating master..."
git checkout master
git reset --hard temp-squash
git branch -D temp-squash
echo "Done."
